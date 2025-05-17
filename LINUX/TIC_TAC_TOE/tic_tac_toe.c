#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>       // shm_open
#include <sys/mman.h>    // mmap
#include <unistd.h>      // ftruncate
#include <pthread.h>
#include <errno.h>
#include <sys/stat.h>    // Для прав доступа

#define BOARD_SIZE 3
#define SHM_NAME "/shared_ttt"

typedef struct {
    char board[BOARD_SIZE][BOARD_SIZE]; // Игровое поле
    int current_turn;                   // 1 - игрок 1, 2 - игрок 2
    int move_count;
    int game_over;                      // 1 - игра окончена
    char winner;                        // 'X', 'O' или '-' (ничья)

    pthread_mutex_t mutex;
    pthread_cond_t cond;
} SharedGame;

void print_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf(" %c ", board[i][j] ? board[i][j] : '.');
            if (j < BOARD_SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < BOARD_SIZE - 1) printf("---+---+---\n");
    }
    printf("\n");
}

char check_winner(char board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        // строки
        if (board[i][0] && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        // столбцы
        if (board[0][i] && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    // диагонали
    if (board[0][0] && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

    return '\0';
}

int main(int argc, char *argv[]) {
    if (argc != 2 || (strcmp(argv[1], "player1") != 0 && strcmp(argv[1], "player2") != 0)) {
        fprintf(stderr, "Usage: %s player1|player2\n", argv[0]);
        exit(1);
    }

    int is_player1 = strcmp(argv[1], "player1") == 0;
    char my_symbol = is_player1 ? 'X' : 'O';
    int my_turn = is_player1 ? 1 : 2;

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    ftruncate(shm_fd, sizeof(SharedGame));
    SharedGame *game = mmap(NULL, sizeof(SharedGame), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (game == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    if (is_player1) {
        memset(game, 0, sizeof(SharedGame));

        pthread_mutexattr_t mattr;
        pthread_condattr_t cattr;

        pthread_mutexattr_init(&mattr);
        pthread_condattr_init(&cattr);

        pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
        pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);

        pthread_mutex_init(&game->mutex, &mattr);
        pthread_cond_init(&game->cond, &cattr);

        game->current_turn = 1;
        game->move_count = 0;
        game->game_over = 0;
        game->winner = '\0';

        pthread_mutexattr_destroy(&mattr);
        pthread_condattr_destroy(&cattr);
    }

    while (1) {
        pthread_mutex_lock(&game->mutex);

        while (!game->game_over && game->current_turn != my_turn) {
            pthread_cond_wait(&game->cond, &game->mutex);
        }

        if (game->game_over) {
            pthread_mutex_unlock(&game->mutex);
            break;
        }

        print_board(game->board);

        int row, col;
        do {
            printf("Player %c, enter your move (row col): ", my_symbol);
            scanf("%d %d", &row, &col);
        } while (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || game->board[row][col]);

        game->board[row][col] = my_symbol;
        game->move_count++;

        char win = check_winner(game->board);
        if (win) {
            game->game_over = 1;
            game->winner = win;
        } else if (game->move_count == 9) {
            game->game_over = 1;
            game->winner = '-';
        } else {
            game->current_turn = 3 - my_turn;
        }

        pthread_cond_broadcast(&game->cond);
        pthread_mutex_unlock(&game->mutex);

        if (game->game_over) break;
    }

    pthread_mutex_lock(&game->mutex);
    if (game->winner == '-') {
        printf("Game over: Draw!\n");
    } else {
        printf("Game over: Player %c wins!\n", game->winner);
    }
    pthread_mutex_unlock(&game->mutex);

    if (is_player1) {
        sleep(2);
        pthread_mutex_destroy(&game->mutex);
        pthread_cond_destroy(&game->cond);
        munmap(game, sizeof(SharedGame));
        shm_unlink(SHM_NAME);
    } else {
        munmap(game, sizeof(SharedGame));
    }

    close(shm_fd);
    return 0;
}


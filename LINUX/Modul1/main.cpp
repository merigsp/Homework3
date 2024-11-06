#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main() {

    int file1 = open("file1.txt", O_RDONLY);
    int file2 = open("file2.txt", O_RDONLY);

    if (file1 == -1 || file2 == -1) {
        cerr << "couldn't open the file" << endl;
        return 1;
    }

    char ch1, ch2;
    int linenum = 1;
    bool diffs = false;

    while (read(file1, &ch1, 1) == 1 && read(file2, &ch2, 1) == 1) {
        if (ch1 != ch2) {
            diffs = true;
            cout << "diffs in line " << linenum  << " "<< ch1 << " - " << ch2 << endl;
        }

        if (ch1 == '\n') {
            linenum++;
        }
    }

    while (read(file1, &ch1, 1) == 1) {
        diffs = true;
        cout << "extra symbol in file1 after line" << linenum  << " " << ch1 << endl;
        if (ch1 == '\n') {
            linenum++;
        }
    }

    while (read(file2, &ch2, 1) == 1) {
        diffs = true;
        cout << "extra symbol in file2 after line" << linenum<< " " << ch2 << endl;
        if (ch2 == '\n') {
            linenum++;
        }
    }

    if (!diffs) {
        cout << "They are the same" << endl;
    }

    close(file1);
    close(file2);

    return 0;
}


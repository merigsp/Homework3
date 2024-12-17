#include "parallel_scheduler.cpp"
#include <iostream>
#include <chrono>
#include <thread>

void tipa_task(int id) {
    std::cout << "Task " << id << " is vazuma " 
              << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
    const size_t capacity = 3; 
    parallel_scheduler scheduler(capacity);

    for (int i = 0; i < 10; ++i) {
        scheduler.run([i]() { tipa_task(i); });
    }

    std::this_thread::sleep_for(std::chrono::seconds(5)); 
    return 0;
}


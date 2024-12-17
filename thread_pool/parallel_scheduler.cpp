#include <iostream>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <vector>

class parallel_scheduler {
public:
    explicit parallel_scheduler(size_t capacity) : stop(false) {
        for (size_t i = 0; i < capacity; ++i) {
            threads.emplace_back([this]() { this->worker_thread(); });
        }
    }

    void run(std::function<void()> func) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            task_queue.push(func);
        }
        cv.notify_one(); 
    }

    ~parallel_scheduler() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        cv.notify_all(); 
        for (std::thread &thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> task_queue;
    std::mutex queue_mutex;
    std::condition_variable cv;
    bool stop;

    void worker_thread() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                cv.wait(lock, [this]() { return stop || !task_queue.empty();});

                if (stop && task_queue.empty()) {
                    return;
                }
                task = task_queue.front();
                task_queue.pop();
            }
            task();
        }
    }
};


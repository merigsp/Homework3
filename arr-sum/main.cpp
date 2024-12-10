#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
#include <numeric>
#include <cstdlib>

void sumArrayPart(const std::vector<int>& array, size_t start, size_t end, long long* KisaSum) {
    *KisaSum = 0;
    for (size_t i = start; i < end; ++i) {
        *KisaSum += array[i];
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Error\n";
        return 1;
    }

    size_t N = std::atoi(argv[1]);
    size_t M = std::atoi(argv[2]);

    if (N <= 1000000 || M < 1) {
        std::cerr << "Error\n";
        return 1;
    }

    std::vector<int> array(N);
    std::mt19937 rng(std::time(0)); 
    std::uniform_int_distribution<int> dist(1, 100);
    for (size_t i = 0; i < N; ++i) {
        array[i] = dist(rng);
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    long long Sum = std::accumulate(array.begin(), array.end(), 0LL);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    startTime = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    std::vector<long long> KisaSum(M, 0);

    size_t MNsize = N / M;

    for (size_t i = 0; i < M; ++i) {
        size_t startIdx = i * MNsize;
        size_t endIdx = (i == M - 1) ? N : startIdx + MNsize;

        threads.emplace_back(sumArrayPart, std::cref(array), startIdx, endIdx, &KisaSum[i]);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    long long Sum_th = 0;
    for (size_t i = 0; i < KisaSum.size(); ++i) {
        Sum_th += KisaSum[i];
    }
    endTime = std::chrono::high_resolution_clock::now();
    auto time_th = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    if (Sum != Sum_th) {
        std::cerr << "Error\n";
        return 1;
    }

    std::cout << "Time spent without threads: " << time << " ms\n";
    std::cout << "Time spent with " << M << " threads: " << time_th << " ms\n";

    return 0;
}


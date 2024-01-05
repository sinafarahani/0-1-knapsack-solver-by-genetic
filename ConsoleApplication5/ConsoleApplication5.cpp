#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "genetic.h"
#include "01knapsack.h"

std::mutex mtx;

int main() {
    int T;
    std::vector<std::thread> solvers;
    std::cin >> T;
    mtx.lock();
    for (int i = 0; i < T; i++) {
        int n, s;
        std::cin >> n >> s;
        knapsack k(n, s);
        genetic g(k);
        auto j = [=]() mutable {g.start(i + 1, std::ref(mtx)); };
        solvers.push_back(std::thread{ j });
    }
    mtx.unlock();
    auto now = std::chrono::high_resolution_clock::now();
    for (auto& w : solvers) {
        w.join();
    }
    auto next = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration<float>(next - now).count();
    std::cout << "calculation took " << time << " seconds" << std::endl;
    system("pause");
}
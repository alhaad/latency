// Mesaure the time of mutex lock and unlock.

#include <iostream>
#include <mutex>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_iterations>" << std::endl;
        return 1;
    }

    int iterations = std::stoi(argv[1]);
    std::mutex mtx;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        mtx.lock();
        mtx.unlock();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << elapsed / iterations << "ns." << std::endl;

    return 0;
}
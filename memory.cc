// Measure the time for main memory reference.
// We need to ensure that we are not causing L1, L2, or L3 cache hits
// by using a linked list that is larger than the cache size.

#include <iostream>
#include <chrono>
#include <list>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_iterations>" << std::endl;
        return 1;
    }

    int iterations = std::stoi(argv[1]);
    std::list<int> lst;

    for (int i = 0; i < iterations; ++i) {
        // TODO: The problem is that we're allocating consecutive memory
        // addresses. Which means that we're causing L1, L2, or L3 cache hits.
        // We need to allocate memory in a way that we're not causing cache
        // hits. Eg. create a large array and then read from random indexes.
        lst.push_back(i);
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < iterations; ++i) {
        lst.pop_front();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    std::cout << elapsed / iterations << "ns." << std::endl;

    return 0;
}
#include <benchmark/benchmark.h>
#include <algorithm>
#include <random>

#define SIZE 1024 * 1024

// Measure the time for main memory reference.
// TODO: Figure out why the number changes with SIZE.
static void BM_MemoryReference(benchmark::State& state) {
    const size_t size = SIZE;
    std::vector<size_t> indices(size);
    
    // Create random permutation for random memory access
    for (size_t i = 0; i < size; i++) {
        indices[i] = i;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);
    
    // Link each element to point to another random element
    std::vector<size_t> data(size);
    for (size_t i = 0; i < size; i++) {
        data[i] = indices[(i + 1) % size];
    }

    size_t index = 0;
    for (auto _ : state) {
        index = data[index];
        benchmark::DoNotOptimize(index);
    }
}
BENCHMARK(BM_MemoryReference);



// Read 1MB of memory sequentially.
static void BM_MemorySequential(benchmark::State& state) {
    int8_t arr[SIZE];

    // While we have allocated 1MB on the stack, reading it sequentially as
    // bytes will be slow. We need to read it as 64-bit integers.
    int64_t* vec = reinterpret_cast<int64_t*>(arr);

    for (auto _ : state) {
        for (int i = 0; i < SIZE / 8; ++i) {
            auto x = vec[i];

            // Prevent the compiler from optimizing out the read.
            benchmark::DoNotOptimize(x);
        }
    }
}
BENCHMARK(BM_MemorySequential);

BENCHMARK_MAIN();
#include <benchmark/benchmark.h>
#include <snappy.h>
#include <string>
#include <random>

static void BM_SnappyCompress1K(benchmark::State& state) {
    // Create 1KB of random data
    const size_t input_size = 1024;  // 1KB
    std::string input(input_size, 0);
    
    // Fill with random data to simulate real-world content
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (size_t i = 0; i < input_size; ++i) {
        input[i] = static_cast<char>(dis(gen));
    }

    // Pre-allocate maximum possible compressed size
    std::string compressed;
    compressed.resize(snappy::MaxCompressedLength(input_size));

    for (auto _ : state) {
        size_t compressed_length;
        snappy::RawCompress(input.data(), input_size, 
                           &compressed[0], &compressed_length);
        benchmark::DoNotOptimize(compressed);
    }

    // Report compression ratio as a counter
    state.counters["CompressionRatio"] = static_cast<double>(compressed.size()) / input_size;
}
BENCHMARK(BM_SnappyCompress1K);

// Alternative version with repeating pattern data
static void BM_SnappyCompress1K_Pattern(benchmark::State& state) {
    // Create 1KB of repeating pattern data
    const size_t input_size = 1024;  // 1KB
    std::string input;
    input.reserve(input_size);
    
    // Create repeating pattern "abcdef..."
    for (size_t i = 0; i < input_size; ++i) {
        input.push_back('a' + (i % 26));
    }

    std::string compressed;
    compressed.resize(snappy::MaxCompressedLength(input_size));

    for (auto _ : state) {
        size_t compressed_length;
        snappy::RawCompress(input.data(), input_size, 
                           &compressed[0], &compressed_length);
        benchmark::DoNotOptimize(compressed);
    }

    state.counters["CompressionRatio"] = static_cast<double>(compressed.size()) / input_size;
}
BENCHMARK(BM_SnappyCompress1K_Pattern);

BENCHMARK_MAIN();
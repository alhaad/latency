#include <benchmark/benchmark.h>
#include <fcntl.h>
#include <unistd.h>
#include <random>
#include <vector>
#include <errno.h>
#include <string.h>

static void BM_Random4KRead(benchmark::State& state) {
    // Open the raw disk device
    int fd = open("/dev/rdisk0", O_RDONLY);
    if (fd < 0) {
        state.SkipWithError(std::string("Failed to open device: ") + strerror(errno));
        return;
    }

    const size_t buffer_size = 4096;  // 4K
    // On macOS, we don't need O_DIRECT, so regular allocation is fine
    char* buffer = new char[buffer_size];

    // Get device size for random positions
    off_t device_size = lseek(fd, 0, SEEK_END);
    if (device_size < 0) {
        state.SkipWithError("Failed to get device size");
        delete[] buffer;
        close(fd);
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<off_t> dis(0, device_size - buffer_size);

    for (auto _ : state) {
        // Generate random position
        off_t pos = dis(gen);
        // Align to 4K boundary
        pos = (pos >> 12) << 12;
        
        // Seek and read
        if (lseek(fd, pos, SEEK_SET) < 0) {
            state.SkipWithError(std::string("Seek failed: ") + strerror(errno));
            break;
        }
        
        auto ret = read(fd, buffer, buffer_size);
        if (ret != buffer_size) {
            state.SkipWithError(std::string("Read failed: ") + strerror(errno));
            break;
        }

        // Prevent the compiler from optimizing away the read
        benchmark::DoNotOptimize(buffer);
        benchmark::ClobberMemory();
    }

    delete[] buffer;
    close(fd);
}

static void BM_Sequential1MBRead(benchmark::State& state) {
    int fd = open("/dev/rdisk0", O_RDONLY);
    if (fd < 0) {
        state.SkipWithError(std::string("Failed to open device: ") + strerror(errno));
        return;
    }

    const size_t buffer_size = 1024 * 1024;  // 1MB
    char* buffer = new char[buffer_size];

    for (auto _ : state) {
        // Reset to beginning for each iteration
        if (lseek(fd, 0, SEEK_SET) < 0) {
            state.SkipWithError(std::string("Seek failed: ") + strerror(errno));
            break;
        }
        
        auto ret = read(fd, buffer, buffer_size);
        if (ret != buffer_size) {
            state.SkipWithError(std::string("Read failed: ") + strerror(errno));
            break;
        }

        // Prevent the compiler from optimizing away the read
        benchmark::DoNotOptimize(buffer);
        benchmark::ClobberMemory();
    }

    delete[] buffer;
    close(fd);
}

BENCHMARK(BM_Random4KRead);
BENCHMARK(BM_Sequential1MBRead);
BENCHMARK_MAIN();
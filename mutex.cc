#include <benchmark/benchmark.h>
#include <mutex>

static void BM_MutexLockUnlock(benchmark::State& state) {
    std::mutex mtx;
    for (auto _ : state) {
        mtx.lock();
        mtx.unlock();
    }
}
BENCHMARK(BM_MutexLockUnlock);

static void BM_MutexLockGuard(benchmark::State& state) {
    std::mutex mtx;
    for (auto _ : state) {
        std::lock_guard<std::mutex> lock(mtx);
    }
}
BENCHMARK(BM_MutexLockGuard);

BENCHMARK_MAIN();
cmake_minimum_required(VERSION 3.13)
project(latency)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(SNAPPY_BUILD_BENCHMARKS OFF CACHE BOOL "Disable snappy benchmarks" FORCE)

# Build benchmark library
add_subdirectory(third_party/benchmark)
add_subdirectory(third_party/snappy)


add_executable(mutex mutex.cc)
target_link_libraries(mutex benchmark::benchmark)

add_executable(memory memory.cc)
target_link_libraries(memory benchmark::benchmark)

add_executable(compress compress.cc)
target_link_libraries(compress benchmark::benchmark snappy)

add_executable(ssd ssd.cc)
target_link_libraries(ssd benchmark::benchmark)
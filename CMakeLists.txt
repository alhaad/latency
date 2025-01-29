cmake_minimum_required(VERSION 3.13)
project(latency)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

# Build benchmark library
add_subdirectory(third_party/benchmark)


add_executable(mutex mutex.cc)
target_link_libraries(mutex benchmark::benchmark)

add_executable(memory memory.cc)
target_link_libraries(memory benchmark::benchmark)
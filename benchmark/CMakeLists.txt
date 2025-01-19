cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(MyBenchmark)

find_package(benchmark REQUIRED)
add_executable(PROJECT_NAME 
    benchmark_test.cpp 
    "../src/admin.cpp" 
    "../src/user.cpp"
    "../src/task.cpp"
)
target_include_directories(PROJECT_NAME PUBLIC "${benchmark_INCLUDE_DIRS}")
target_link_libraries(PROJECT_NAME "${benchmark_LIBRARIES}")
target_link_libraries(PROJECT_NAME benchmark::benchmark)

# target_compile_options(MyBenchmark PRIVATE -O3)



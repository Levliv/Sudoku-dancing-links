cmake_minimum_required(VERSION 3.26)
project(SudokuDLXSolver)

set(CMAKE_CXX_STANDARD 17)

add_executable(DLXSolver_run main.cpp)


include_directories(DLXSolver_lib)
add_subdirectory(DLXSolver_lib)

target_link_libraries(DLXSolver_run DLXSolver_lib)

enable_testing()
add_subdirectory(ctest)

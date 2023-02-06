cmake_minimum_required(VERSION 3.13...3.20)
project(analysis-tools)
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to conform to")


set(Clang_DIR ${CLANG_DIR})
message(STATUS "Searching for ClangConfig.cmake in: ${Clang_DIR}")
find_package(Clang REQUIRED CONFIG)


message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using ClangConfig.cmake in: ${CLANG_DIR}")


list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CLANG_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")



include(TableGen)
include(AddLLVM)
include(AddClang)
include(HandleLLVMOptions)


include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)
link_directories(${LLVM_BUILD_LIBRARY_DIR})
add_definitions(${LLVM_DEFINITIONS})


message(STATUS ${LLVM_BUILD_LIBRARY_DIR})
set(LLVM_LIT_ARGS "-sv" CACHE STRING "lit default options")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

enable_testing()

add_subdirectory(logEventFinder)
add_subdirectory(tests)
add_subdirectory(applications)

cmake_minimum_required(VERSION 3.5.1)

project(btree-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(btree_test)

target_sources(btree_test
 PRIVATE 
  "src/btree_test.cpp"
  "src/btree.cpp"
  )

target_include_directories(btree_test
 PRIVATE 
 "${CMAKE_SOURCE_DIR}/src"
)

target_include_directories(btree_test PRIVATE "${PROJECT_SOURCE_DIR}/src")

cmake_minimum_required(VERSION 3.8)
project(Lecture2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -Wall -ggdb3")

set(LIB_SOURCE_FILES
        DArray.cpp
        LinkedList.cpp
        Queue.cpp
        Stack.cpp
        Heap.cpp)

add_library(study_std_lib STATIC ${LIB_SOURCE_FILES})

add_executable(prog main.cpp)
add_executable(tests_cppunit tests_cppunit.cpp)

target_link_libraries(prog study_std_lib)
target_link_libraries(tests_cppunit study_std_lib)

include_directories(3rdparty/cppunit/include)

find_library(CPPUNIT_LIB NAMES cppunit cppunitd PATHS /usr/local/lib)
set(CPPUNIT_LIBRARY ${CPPUNIT_LIB})
target_link_libraries(tests_cppunit ${CPPUNIT_LIBRARY})

# sets GTEST_INCLUDE_DIRS and GTEST_LIBRARIES
find_package( GTest REQUIRED )
# adds the gtest include directory
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests_gtest tests_gtest.cpp)

# links gtest
target_link_libraries(tests_gtest ${GTEST_LIBRARIES})
target_link_libraries(tests_gtest study_std_lib)

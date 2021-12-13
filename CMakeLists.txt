cmake_minimum_required(VERSION 3.21)
project(graph2)

set(CMAKE_CXX_STANDARD 23)

enable_language(C)
enable_language(CXX)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "-Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused")
    set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif ()

include_directories(
        ${PROJECT_SOURCE_DIR}/src
)

add_library(
        example
        src/main.cpp src/vertex.hpp src/graph.hpp src/polyhedron.hpp src/face.hpp src/surface.hpp)

set(GOOGLETEST_ROOT gtest/googletest CACHE STRING "Google Test source root")

include_directories(
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
)

set(GOOGLETEST_SOURCES
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
        )

foreach (_source ${GOOGLETEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach ()

add_library(googletest ${GOOGLETEST_SOURCES})

add_executable(
        unit_tests
        test/main.cpp
        test/tests.cpp src/vertex.hpp src/graph.hpp src/polyhedron.hpp src/face.hpp src/surface.hpp)

add_executable(
        graph
        src/main.cpp src/vertex.hpp src/graph.hpp src/polyhedron.hpp src/face.hpp src/surface.hpp)

add_dependencies(unit_tests googletest)

target_link_libraries(
        unit_tests
        googletest
        example
        pthread
)

include(CTest)
enable_testing()

add_test(unit ${PROJECT_BINARY_DIR}/unit_tests)

#add_executable(graph main.cpp graph.hpp polyhedron.hpp face.hpp vertex.hpp surface.hpp)

project(CPPTutorial)
cmake_minimum_required(VERSION 2.6.2)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall -Wextra -Wno-deprecated-declarations -pedantic" )
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall -Wextra -Wno-deprecated-declarations -Wno-ignored-attributes -Wno-misleading-indentation -Wno-unknown-pragmas -pedantic" )
endif()

add_executable(tutorial cpptutorial.cpp)
add_executable(myvector myvector.cpp)
add_executable(lambdatransform lambdatransform.cpp)
add_executable(fnvec fnvec.cpp)
add_executable(tietuple tietuple.cpp)
add_executable(cppdemo cppdemo.cpp)
add_executable(fnrefinclass fnrefinclass fnrefinclass.cpp)
add_executable(recorderdemo recorderdemo.cpp recorderdemo.h)

cmake_minimum_required(VERSION 2.8)
project(substenv)

add_executable_numcse(main main.cpp)


include_directories(${DIRS}
	../../rowbandwidth/Eigen
	)


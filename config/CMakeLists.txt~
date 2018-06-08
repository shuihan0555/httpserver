cmake_minimum_required(VERSION 2.8)
project(xflagger)

find_package(gflags REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")


include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILE main.cc 
	src/xconfig.cc
	#src/solution_config.cc
	src/marketsolution_config.cc
	)

add_executable(xflagger ${SOURCE_FILE})

target_link_libraries(xflagger gflags)

cmake_minimum_required(VERSION 2.8) 

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
find_package(Boost 1.58.0)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	add_executable(main src/main.cpp)
endif()

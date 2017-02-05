cmake_minimum_required(VERSION 2.8) 

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(BOOST_INCLUDE_DIR /usr/include)
set(BOOST_LIB_DIR /usr/lib)
set(BOOST_LIB libboost_system.a)

include_directories(${BOOST_INCLUDE_DIR})
link_directories(${BOOST_LIB_DIR})
add_executable(main main.cpp)
target_link_libraries(${TARGET} ${BOOST_LIB})


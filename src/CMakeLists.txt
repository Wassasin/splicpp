cmake_minimum_required(VERSION 2.6)

project(splicpp)

include_directories("${PROJECT_BINARY_DIR}")

add_executable(splicpp
	main.cpp
)

add_definitions("-Wall -Wextra -Weffc++ -std=c++0x")

find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(splicpp ${Boost_SYSTEM_LIBRARY})

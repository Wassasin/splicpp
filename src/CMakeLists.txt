cmake_minimum_required(VERSION 2.6)

project(splicpp)

include_directories("${PROJECT_BINARY_DIR}")

add_executable(splicpp
	main.cpp
)

add_definitions("-Wall -Wextra -Weffc++ -std=c++0x -g3 -fno-inline -O0")

find_package(Boost COMPONENTS system program_options regex REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(splicpp ${Boost_SYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_REGEX_LIBRARY})

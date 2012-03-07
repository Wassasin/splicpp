cmake_minimum_required(VERSION 2.6)

project(splicpp)

include_directories("${PROJECT_BINARY_DIR}")

add_executable(splicpp
	main.cpp
	
	common/grammar.cpp
	common/rule.cpp
	common/symbols.cpp
	common/token.cpp

	parser/bnf_parser.cpp	
	parser/bnf_bootstrap_parser.cpp
	parser/cst_element.cpp
	parser/cst_node.cpp
	parser/lexer.cpp
	parser/ptable.cpp
	parser/spl_parser.cpp
)

macro(add_grammar SRC_FILE)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/grammars/${SRC_FILE}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/grammars/${SRC_FILE} ${CMAKE_CURRENT_BINARY_DIR}/grammars/${SRC_FILE}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/grammars/${SRC_FILE}
		COMMENT "Copying grammar ${SRC_FILE}"
	)
	add_custom_target(${SRC_FILE} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/grammars/${SRC_FILE})
	add_dependencies(splicpp ${SRC_FILE})
endmacro()

add_grammar(bnf.bnf)
add_grammar(test.bnf)
add_grammar(spl.bnf)

add_definitions("-Wall -Wextra -Weffc++ -std=c++0x -g3 -fno-inline -O0")

find_package(Boost COMPONENTS system program_options regex filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(splicpp ${Boost_SYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

cmake_minimum_required(VERSION 2.6)

project(splicpp)

include_directories("${PROJECT_BINARY_DIR}")

add_executable(splicpp
	main.cpp
	
	common/grammar.cpp
	common/rule.cpp
	common/symbols.cpp
	common/sloc.cpp
	common/token.cpp

	parser/bnf_parser.cpp	
	parser/bnf_bootstrap_parser.cpp
	parser/cst_element.cpp
	parser/cst_node.cpp
	parser/lexer.cpp
	parser/ptable.cpp
	parser/spl_parser.cpp
	parser/clr_parser_gen.cpp
	
	ast/ast.cpp
	ast/ast_construct.cpp
	ast/ast_decl.cpp
	ast/ast_exp.cpp
	ast/ast_f_arg.cpp
	ast/ast_fun_call.cpp
	ast/ast_fun_decl.cpp
	ast/ast_id.cpp
	ast/ast_prog.cpp
	ast/ast_stmt.cpp
	ast/ast_type.cpp
	ast/ast_var_decl.cpp
	
	typing/typecontext.cpp
	typing/ltypecontext.cpp
	typing/substitution.cpp
	typing/symboltable.cpp
	typing/varcontext.cpp
	
	typing/types/sl_polytype.cpp
	typing/types/sl_type.cpp
	typing/types/sl_type_array.cpp
	typing/types/sl_type_bool.cpp
	typing/types/sl_type_function.cpp
	typing/types/sl_type_int.cpp
	typing/types/sl_type_tuple.cpp
	typing/types/sl_type_unbound.cpp
	typing/types/sl_type_void.cpp
	
	ir/ircontext.cpp
	ir/ir_exp.cpp
	ir/ir_exp_binop.cpp
	ir/ir_exp_const.cpp
	ir/ir_exp_eseq.cpp
	ir/ir_exp_mem.cpp
	ir/ir_exp_name.cpp
	ir/ir_exp_temp.cpp
	ir/ir_stmt.cpp
	ir/ir_stmt_cjump.cpp
	ir/ir_stmt_jump.cpp
	ir/ir_stmt_label.cpp
	ir/ir_stmt_move.cpp
	ir/ir_stmt_seq.cpp
	
	mappers/ir_desequencer.cpp
	mappers/ir_label_analyser.cpp
	mappers/ir_temp_analyser.cpp
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
add_grammar(spl.bnf)

add_definitions("-Wall -Wextra -Weffc++ -std=c++0x -pedantic -g3 -O3")

find_package(Boost COMPONENTS system program_options regex filesystem serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(splicpp ${Boost_SYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SERIALIZATION_LIBRARY})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cache)

#ifndef SPL_H
#define SPL_H

#include <boost/algorithm/string/trim.hpp>

#include "bnf_parser.hpp"
#include "../common/io.hpp"

namespace splicpp
{
	class spl : public grammar
	{
	public:
		const stid C_LINE, C_BLOCK, L_VOID, L_BOOL, L_INT, L_FALSE, L_TRUE, L_IF, L_ELSE, L_WHILE, L_RETURN, L_SEMICOLON, L_BRACKET_LEFT, L_BRACKET_RIGHT, L_CBRACKET_LEFT, L_CBRACKET_RIGHT, L_SBRACKET_LEFT, L_SBRACKET_RIGHT, L_COMMA, L_ID, L_DIGIT, L_MINUS, L_PLUS, L_TIMES, L_DIVIDES, L_MOD, L_DISJUNCTION, L_CONJUNCTION, L_LEQ, L_GEQ, L_EQ, L_NEQ, L_LESSER, L_GREATER, L_ASSIGNMENT, L_NEGATION, L_COLON, NL_PROG;
	
		spl()
		: grammar()
		
		, C_LINE(add_symbol(				new line_comment_literal("c_line", "//")))
		, C_BLOCK(add_symbol(				new block_comment_literal("c_block", "/*", "*/")))
		
		, L_VOID(add_symbol(				new regex_literal("l_void", "(\\<Void\\>)")))
		, L_BOOL(add_symbol(				new regex_literal("l_bool", "(\\<Bool\\>)")))
		, L_INT(add_symbol(					new regex_literal("l_int", "(\\<Int\\>)")))
		, L_FALSE(add_symbol(				new regex_literal("l_false", "(\\<False\\>)")))
		, L_TRUE(add_symbol(				new regex_literal("l_true", "(\\<True\\>)")))
		
		, L_IF(add_symbol(					new regex_literal("l_if", "(\\<if\\>)")))
		, L_ELSE(add_symbol(				new regex_literal("l_else", "(\\<else\\>)")))
		, L_WHILE(add_symbol(				new regex_literal("l_while", "(\\<while\\>)")))
		, L_RETURN(add_symbol(				new regex_literal("l_return", "(\\<return\\>)")))
		
		, L_SEMICOLON(add_symbol(			new static_literal("l_semicolon", ";")))
		, L_BRACKET_LEFT(add_symbol(		new static_literal("l_bracket_left", "(")))
		, L_BRACKET_RIGHT(add_symbol(		new static_literal("l_bracket_right", ")")))
		, L_CBRACKET_LEFT(add_symbol(		new static_literal("l_cbracket_left", "{")))
		, L_CBRACKET_RIGHT(add_symbol(		new static_literal("l_cbracket_right", "}")))
		, L_SBRACKET_LEFT(add_symbol(		new static_literal("l_sbracket_left", "[")))
		, L_SBRACKET_RIGHT(add_symbol(		new static_literal("l_sbracket_right", "]")))
		, L_COMMA(add_symbol(				new static_literal("l_comma", ",")))

		, L_ID(add_symbol(					new regex_literal("l_id", "([a-zA-Z][a-zA-Z0-9\\_]*)")))
		, L_DIGIT(add_symbol(				new regex_literal("l_digit", "(0|[1-9][0-9]*)")))
		
		, L_MINUS(add_symbol(				new static_literal("l_minus", "-")))
		, L_PLUS(add_symbol(				new static_literal("l_plus", "+")))
		
		, L_TIMES(add_symbol(				new static_literal("l_times", "*")))
		, L_DIVIDES(add_symbol(				new static_literal("l_divides", "/")))
		, L_MOD(add_symbol(					new static_literal("l_mod", "%")))
		
		, L_DISJUNCTION(add_symbol(			new static_literal("l_disjunction", "||")))
		, L_CONJUNCTION(add_symbol(			new static_literal("l_conjunction", "&&")))
		
		, L_LEQ(add_symbol(					new static_literal("l_leq", "<=")))
		, L_GEQ(add_symbol(					new static_literal("l_geq", ">=")))
		, L_EQ(add_symbol(					new static_literal("l_eq", "==")))
		, L_NEQ(add_symbol(					new static_literal("l_neq", "!=")))
		, L_LESSER(add_symbol(				new static_literal("l_lesser", "<")))
		, L_GREATER(add_symbol(				new static_literal("l_greater", ">")))
		
		, L_ASSIGNMENT(add_symbol(			new static_literal("l_assignment", "=")))
		, L_NEGATION(add_symbol(			new static_literal("l_negation", "!")))
		, L_COLON(add_symbol(				new static_literal("l_colon", ":")))

		, NL_PROG(add_symbol(				new non_literal("nl_prog")))
		{		
			ignore(' ');
			ignore('\t');
			ignore('\n');
			
			splicpp::bnf_parser p;
			
			std::string lang = readfile("grammars/spl.bnf");
			boost::algorithm::trim(lang);
			
			p.parse((*this), lang);
			
			add_rule(rule(NL_START) + NL_PROG);
			
			check();
			
			//print();
		}
	};
}

#endif

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
		const stid L_VOID, L_BOOL, L_INT, L_IF, L_ELSE, L_WHILE, L_RETURN, L_ASSIGNMENT, L_SEMICOLON, L_BRACKETS_LEFT, L_BRACKETS_RIGHT, L_CBRACKET_LEFT, L_CBRACKET_RIGHT, L_SBRACKET_LEFT, L_SBRACKET_RIGHT, L_COMMA, L_UNDERSCORE, L_ID, NL_PROG;
	
		spl()
		: grammar()
		
		, L_VOID(add_symbol(				new regex_literal("l_void", "(\\<Void\\>)")))
		, L_BOOL(add_symbol(				new regex_literal("l_bool", "(\\<Bool\\>)")))
		, L_INT(add_symbol(					new regex_literal("l_int", "(\\<Int\\>)")))
		
		, L_IF(add_symbol(					new regex_literal("l_if", "(\\<if\\>)")))
		, L_ELSE(add_symbol(				new regex_literal("l_else", "(\\<else\\>)")))
		, L_WHILE(add_symbol(				new regex_literal("l_while", "(\\<while\\>)")))
		, L_RETURN(add_symbol(				new regex_literal("l_return", "(\\<return\\>)")))
		
		, L_ASSIGNMENT(add_symbol(			new static_literal("l_assignment", "=")))
		, L_SEMICOLON(add_symbol(			new static_literal("l_semicolon", ";")))
		, L_BRACKETS_LEFT(add_symbol(		new static_literal("l_brackets_left", "(")))
		, L_BRACKETS_RIGHT(add_symbol(		new static_literal("l_brackets_right", ")")))
		, L_CBRACKET_LEFT(add_symbol(		new static_literal("l_cbracket_left", "{")))
		, L_CBRACKET_RIGHT(add_symbol(		new static_literal("l_cbracket_right", "}")))
		, L_SBRACKET_LEFT(add_symbol(		new static_literal("l_sbracket_left", "[")))
		, L_SBRACKET_RIGHT(add_symbol(		new static_literal("l_sbracket_right", "]")))
		, L_COMMA(add_symbol(				new static_literal("l_comma", ",")))
		, L_UNDERSCORE(add_symbol(			new static_literal("l_underscore", "_")))
		
		, L_ID(add_symbol(					new regex_literal("l_id", "([a-zA-Z][a-zA-Z0-9]*[\\_]?)")))

		, NL_PROG(add_symbol(		new non_literal("nl_prog")))
		{
			ignore(' ');
			ignore('\t');
			
			splicpp::bnf_parser p;
			std::string lang = readfile("grammars/spl.bnf");
			boost::algorithm::trim(lang);
			
			p.parse((*this), lang);
			
			add_rule(rule(NL_START) + NL_PROG);
			
			std::cout << std::endl;
			print();
			
			//check();
		}
	};
}

#endif

#ifndef BNF_PARSER_H
#define BNF_PARSER_H

#include "bnf.hpp"
#include "ptable.hpp"
#include "slr_parser_gen.hpp"

namespace splicpp
{
	class bnf_parser
	{
		const bnf g_bnf;
		const ptable t;
	
		/*	std::string lang = 
		"syntax		:== rule | syntax newline rule\n"
		"rule		:== rule-name assignment expr\n"
		"expr		:== list | expr expr-sep list\n"
		"list		:== rule-name | list rule-name";*/
		
		stid try_add_symbol(grammar& g, const std::string name) const;
		void parse_syntax(grammar& g, const std::string str, const cst_node n) const;
		void parse_rule(grammar& g, const std::string str, const cst_node n) const;
		void parse_expr(grammar& g, const std::string str, const cst_node n, const stid start) const;
		void parse_list(grammar& g, const std::string str, const cst_node n, rule& r) const;
		std::string parse_rule_name(const std::string str, const token t) const;
	
	public:
		bnf_parser()
		: g_bnf()
		, t(splicpp::slr_parser_gen::generate(g_bnf))
		{}
		
		void parse(grammar& g, const std::string lang);
	};
}

#endif

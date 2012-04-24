#ifndef BNF_PARSER_H
#define BNF_PARSER_H

#include "bnf.hpp"
#include "ptable.hpp"

namespace splicpp
{
	class bnf_parser
	{
		const bnf g_bnf;
		const ptable t;
	
		stid try_add_symbol(grammar& g, const std::string name) const;
	
		stid parse_syntax(grammar& g, const std::string str, const cst_node n) const;
		stid parse_rule(grammar& g, const std::string str, const cst_node n) const;
		void parse_alt_rule(grammar& g, const std::string str, const cst_node n, stid start) const;
		void parse_expr(grammar& g, const std::string str, const cst_node n, rule& r) const;
		void parse_symbol(grammar& g, const std::string str, const cst_node n, rule& r) const;
		
		stid add_optional(grammar& g, std::string rule_name, const stid s) const;
		stid add_kleene(grammar& g, std::string rule_name, const stid s) const;
		stid add_plus(grammar& g, std::string rule_name, const stid s) const;
		
		std::string parse_rule_name(const std::string str, const s_ptr<cst_element> e) const;
	public:
		bnf_parser()
		: g_bnf()
		, t(fetch_ptable(g_bnf))
		{}
		
		void parse(grammar& g, const std::string lang);
		
		static ptable fetch_ptable(const grammar& g);
	};
}

#endif

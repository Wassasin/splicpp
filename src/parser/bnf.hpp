#ifndef BNF_H
#define BNF_H

#include <boost/algorithm/string/trim.hpp>

#include "bnf_bootstrap_parser.hpp"
#include "../common/io.hpp"

namespace splicpp
{
	class bnf : public grammar
	{
	public:
		const stid L_RULE_NAME, L_ASSIGNMENT, L_EXPR_SEP, L_OPTIONAL, L_KLEENE, L_PLUS, L_NEWLINE, NL_SYNTAX;
	
		bnf()
		: grammar()

		, L_RULE_NAME(add_symbol(	new regex_literal("l_rule_name", "([a-zA-Z0-9\\-]+)")))
		, L_ASSIGNMENT(add_symbol(	new static_literal("l_assignment", ":==")))
		, L_EXPR_SEP(add_symbol(	new static_literal("l_expr_sep", "|")))
		, L_OPTIONAL(add_symbol(	new static_literal("l_optional", "?")))
		, L_KLEENE(add_symbol(		new static_literal("l_kleene", "*")))
		, L_PLUS(add_symbol(		new static_literal("l_plus", "+")))
		, L_NEWLINE(add_symbol(		new static_literal("l_newline", "\n")))
		
		, NL_SYNTAX(add_symbol(		new non_literal("nl_syntax")))
		{
			ignore(' ');
			ignore('\t');
			
			splicpp::bnf_bootstrap_parser p;
			std::string lang = readfile("grammars/bnf.bnf");
			boost::algorithm::trim(lang);
			
			p.parse((*this), lang);
			
			add_rule(rule(NL_START) + NL_SYNTAX);
		}
	};
}

#endif

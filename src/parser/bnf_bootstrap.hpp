#ifndef BNF_BOOTSTRAP_H
#define BNF_BOOTSTRAP_H

#include "../common/grammar.hpp"
#include "../common/symbols.hpp"
#include "../common/rule.hpp"

namespace splicpp
{
	class bnf_bootstrap : public grammar
	{
	public:
		const stid L_RULE_NAME, L_ASSIGNMENT, L_NEWLINE, NL_SYNTAX, NL_RULE, NL_EXPR;
	
		bnf_bootstrap()
		: grammar()

		, L_RULE_NAME(add_symbol(	new regex_literal("l_rule_name", "([a-zA-Z0-9\\-\\_]+)")))
		, L_ASSIGNMENT(add_symbol(	new static_literal("l_assignment", ":==")))
		, L_NEWLINE(add_symbol(		new static_literal("l_newline", "\n")))
		
		, NL_SYNTAX(add_symbol(		new non_literal("nl_syntax")))
		, NL_RULE(add_symbol(		new non_literal("nl_rule")))
		, NL_EXPR(add_symbol(		new non_literal("nl_expr")))
		{
			add_rule(rule(NL_START) + NL_SYNTAX);

			add_rule(rule(NL_SYNTAX) + NL_RULE);
			add_rule(rule(NL_SYNTAX) + NL_SYNTAX + L_NEWLINE + NL_RULE);
			add_rule(rule(NL_RULE) + L_RULE_NAME + L_ASSIGNMENT + NL_EXPR);
			add_rule(rule(NL_EXPR));
			add_rule(rule(NL_EXPR) + NL_EXPR + L_RULE_NAME);

			ignore(' ');
			ignore('\t');
		}
	};
}

#endif

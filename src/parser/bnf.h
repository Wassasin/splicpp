#ifndef BNF_H
#define BNF_H

#include "../common/grammar.h"
#include "../common/symbols.h"
#include "../common/rule.h"

namespace splicpp
{
	class bnf : public grammar
	{
		const stid L_RULE_NAME, L_ASS, L_EXPR_SEP, L_NEWLINE, NL_SYNTAX, NL_RULE, NL_EXPR, NL_LIST;
		
	public:
		bnf()
		: grammar()

		, L_RULE_NAME(add_symbol(	new regex_literal("rule-name", "([a-zA-Z0-9\\-]+)")))
		, L_ASS(add_symbol(			new static_literal("assignment", ":==")))
		, L_EXPR_SEP(add_symbol(	new static_literal("expr-sep", "|")))
		, L_NEWLINE(add_symbol(		new static_literal("newline", "\n")))
		
		, NL_SYNTAX(add_symbol(		new non_literal("syntax")))
		, NL_RULE(add_symbol(		new non_literal("rule")))
		, NL_EXPR(add_symbol(		new non_literal("expr")))
		, NL_LIST(add_symbol(		new non_literal("list")))
		{			
			add_rule(rule(NL_SYNTAX) + NL_RULE);
			add_rule(rule(NL_SYNTAX) + NL_SYNTAX + L_NEWLINE + NL_RULE);
			add_rule(rule(NL_RULE) + L_RULE_NAME + L_ASS + NL_EXPR);
			add_rule(rule(NL_EXPR) + NL_LIST);
			add_rule(rule(NL_EXPR) + NL_EXPR + L_EXPR_SEP + NL_LIST);
			add_rule(rule(NL_LIST) + L_RULE_NAME);
			add_rule(rule(NL_LIST) + NL_LIST + L_RULE_NAME);

			ignore(' ');
			ignore('\t');
		}
	};
}

#endif

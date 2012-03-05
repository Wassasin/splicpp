#ifndef BNF_PARSER_H
#define BNF_PARSER_H

#include "bnf.h"
#include "ptable.h"
#include "slr_parser_gen.h"

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
		
		stid try_add_symbol(grammar& g, const std::string name) const
		{
			for(stid i = 0; i < g.symbols_size(); i++)
				if(g.fetch_symbol(i)->name == name)
					return i;
			
			return g.add_symbol(new non_literal(name));
		}
		
		void parse_syntax(grammar& g, const std::string str, const cst_node n) const //syntax :== rule | syntax newline rule
		{
			n.assert_stid(g_bnf, "syntax");
			
			if(n.size() == 1)
				parse_rule(g, str, n[0]->as_node());
			else if(n.size() == 3)
			{
				parse_syntax(g, str, n[0]->as_node());
				n[1]->assert_token(g_bnf, "newline");
				parse_rule(g, str, n[2]->as_node());
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		void parse_rule(grammar& g, const std::string str, const cst_node n) const //rule :== rule-name assignment expr
		{
			n.assert_stid(g_bnf, "rule");
			assert(n.size() == 3);
			
			n[0]->assert_token(g_bnf, "rule-name");
			stid s = try_add_symbol(g, parse_rule_name(str, n[0]->as_token()));
			n[1]->assert_token(g_bnf, "assignment");
			parse_expr(g, str, n[2]->as_node(), s);
		}
		
		void parse_expr(grammar& g, const std::string str, const cst_node n, const stid start) const //expr :== list | expr expr-sep list
		{
			n.assert_stid(g_bnf, "expr");
			rule r(start);
			
			if(n.size() == 1)
				parse_list(g, str, n[0]->as_node(), r);
			else if(n.size() == 3)
			{
				parse_expr(g, str, n[0]->as_node(), start);
				n[1]->assert_token(g_bnf, "expr-sep");
				parse_list(g, str, n[2]->as_node(), r);
			}
			else
				throw std::logic_error("unexpected rule");
				
			g.add_rule(r);
		}
		
		void parse_list(grammar& g, const std::string str, const cst_node n, rule& r) const //list :== rule-name | list rule-name
		{
			n.assert_stid(g_bnf, "list");
			
			if(n.size() == 1)
				r.append_body(try_add_symbol(g, parse_rule_name(str, n[0]->as_token())));
			else if(n.size() == 2)
			{
				parse_list(g, str, n[0]->as_node(), r);
				r.append_body(try_add_symbol(g, parse_rule_name(str, n[1]->as_token())));
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		std::string parse_rule_name(const std::string str, const token t) const
		{
			return t.as_string(str);
		}
	
	public:
		bnf_parser()
		: g_bnf()
		, t(splicpp::slr_parser_gen::generate(g_bnf))
		{}
		
		void parse(grammar& g, const std::string lang)
		{
			splicpp::lexer l(g_bnf, lang);
			cst_element e = t.parse(l);
			
			parse_syntax(g, lang, e.as_node()[0]->as_node());
		}
	};
}

#endif

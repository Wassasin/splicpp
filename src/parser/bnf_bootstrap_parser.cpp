#include "bnf_bootstrap_parser.hpp"

namespace splicpp
{
		stid bnf_bootstrap_parser::try_add_symbol(grammar& g, const std::string name) const
		{
			for(stid i = 0; i < g.symbols_size(); i++)
				if(g.fetch_symbol(i)->name == name)
					return i;
			
			return g.add_symbol(new non_literal(name));
		}
		
		void bnf_bootstrap_parser::parse_syntax(grammar& g, const std::string str, const cst_node n) const //syntax :== rule | syntax newline rule
		{
			n.assert_stid(g_bnf, "nl_syntax");
			
			if(n.size() == 1)
				parse_rule(g, str, n[0]->as_node());
			else if(n.size() == 3)
			{
				parse_syntax(g, str, n[0]->as_node());
				n[1]->assert_token(g_bnf, "l_newline");
				parse_rule(g, str, n[2]->as_node());
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		void bnf_bootstrap_parser::parse_rule(grammar& g, const std::string str, const cst_node n) const //rule :== rule-name assignment expr
		{
			n.assert_stid(g_bnf, "nl_rule");
			assert(n.size() == 3);
			
			stid s = try_add_symbol(g, parse_rule_name(str, n[0]));
			n[1]->assert_token(g_bnf, "l_assignment");
			
			rule r(s);
			parse_expr(g, str, n[2]->as_node(), r);
			g.add_rule(r);
		}
		
		void bnf_bootstrap_parser::parse_expr(grammar& g, const std::string str, const cst_node n, rule& r) const //expr :== | expr rule-name
		{
			n.assert_stid(g_bnf, "nl_expr");
			
			if(n.size() == 0)
			{}
			else if(n.size() == 2)
			{
				parse_expr(g, str, n[0]->as_node(), r);
				r.append_body(try_add_symbol(g, parse_rule_name(str, n[1])));
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		std::string bnf_bootstrap_parser::parse_rule_name(const std::string str, const std::shared_ptr<cst_element> e) const
		{
			e->assert_token(g_bnf, "l_rule_name");
			return e->as_token().as_string(str);
		}
		
		void bnf_bootstrap_parser::parse(grammar& g, const std::string lang)
		{
			splicpp::lexer l(g_bnf, lang);
			cst_element e = t.parse(l);
			
			parse_syntax(g, lang, e.as_node()[0]->as_node());
		}
}

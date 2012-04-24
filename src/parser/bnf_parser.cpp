#include "bnf_parser.hpp"

#include "clr_parser_gen.hpp"

namespace splicpp
{
		stid bnf_parser::try_add_symbol(grammar& g, const std::string name) const
		{
			for(stid i = 0; i < g.symbols_size(); i++)
				if(g.fetch_symbol(i)->name == name)
					return i;
			
			return g.add_symbol(new non_literal(name));
		}
	
		stid bnf_parser::parse_syntax(grammar& g, const std::string str, const cst_node n) const //nl_syntax :== nl_rule | nl_syntax nl_newlines nl_rule | nl_syntax nl_newlines nl_alt_rule
		{
			n.assert_stid(g_bnf, "nl_syntax");
			
			if(n.size() == 1)
				return parse_rule(g, str, n[0]->as_node());
			else if(n.size() == 3)
			{
				stid s = parse_syntax(g, str, n[0]->as_node());
				n[1]->assert_node(g_bnf, "nl_newlines");
			
				if(n[2]->is_node(g_bnf, "nl_rule"))
					return parse_rule(g, str, n[2]->as_node());
				else if(n[2]->is_node(g_bnf, "nl_alt_rule"))
				{
					parse_alt_rule(g, str, n[2]->as_node(), s);
					return s;
				}
			}
			
			throw std::logic_error("unexpected rule");
		}
		
		stid bnf_parser::parse_rule(grammar& g, const std::string str, const cst_node n) const
		{
			n.assert_stid(g_bnf, "nl_rule");
			assert(n.size() == 3);
			
			stid s = try_add_symbol(g, parse_rule_name(str, n[0]));
			n[1]->assert_token(g_bnf, "l_assignment");
			
			rule r(s);
			parse_expr(g, str, n[2]->as_node(), r);
			g.add_rule(r);
			
			return s;
		}
		
		void bnf_parser::parse_alt_rule(grammar& g, const std::string str, const cst_node n, const stid start) const
		{
			n.assert_stid(g_bnf, "nl_alt_rule");
			assert(n.size() == 2);
			
			n[0]->assert_token(g_bnf, "l_expr_sep");
			
			rule r(start);
			parse_expr(g, str, n[1]->as_node(), r);
			g.add_rule(r);
		}
		
		void bnf_parser::parse_expr(grammar& g, const std::string str, const cst_node n, rule& r) const
		{
			n.assert_stid(g_bnf, "nl_expr");
		
			if(n.size() == 1)
				parse_symbol(g, str, n[0]->as_node(), r);
			else if(n.size() == 2)
			{
				parse_expr(g, str, n[0]->as_node(), r);
				parse_symbol(g, str, n[1]->as_node(), r);
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		void bnf_parser::parse_symbol(grammar& g, const std::string str, const cst_node n, rule& r) const
		{
			n.assert_stid(g_bnf, "nl_symbol");
		
			if(n.size() == 1)
				r.append_body(try_add_symbol(g, parse_rule_name(str, n[0])));
			else if(n.size() == 2)
			{
				std::string rule_name = parse_rule_name(str, n[0]);
				stid s = try_add_symbol(g, rule_name);
				
				if(n[1]->is_token(g_bnf, "l_optional"))
					r.append_body(add_optional(g, rule_name, s));
				else if(n[1]->is_token(g_bnf, "l_kleene"))
					r.append_body(add_kleene(g, rule_name, s));
				else if(n[1]->is_token(g_bnf, "l_plus"))
					r.append_body(add_plus(g, rule_name, s));
			}
		}
		
		stid bnf_parser::add_optional(grammar& g, std::string rule_name, const stid s) const
		{
			rule_name.append("_opt");
			stid s_new = try_add_symbol(g, rule_name);
			
			g.add_rule(rule(s_new));
			g.add_rule(rule(s_new) + s);
			
			return s_new;
		}
		
		stid bnf_parser::add_kleene(grammar& g, std::string rule_name, const stid s) const
		{
			rule_name.append("_kleene");
			stid s_new = try_add_symbol(g, rule_name);
			
			g.add_rule(rule(s_new));
			g.add_rule(rule(s_new) + s_new + s);
			
			return s_new;
		}
		
		stid bnf_parser::add_plus(grammar& g, std::string rule_name, const stid s) const
		{
			rule_name.append("_plus");
			stid s_new = try_add_symbol(g, rule_name);
			
			g.add_rule(rule(s_new) + s);
			g.add_rule(rule(s_new) + s_new + s);
			
			return s_new;
		}
		
		std::string bnf_parser::parse_rule_name(const std::string str, const s_ptr<cst_element> e) const
		{
			e->assert_token(g_bnf, "l_rule_name");
			return e->as_token().as_string(str);
		}

		void bnf_parser::parse(grammar& g, const std::string lang)
		{
			splicpp::lexer l(g_bnf, lang);
			cst_element e = t.parse(l);
			
			parse_syntax(g, lang, e.as_node()[0]->as_node());
		}
		
		ptable bnf_parser::fetch_ptable(const grammar& g)
		{
			return clr_parser_gen::generate(g);
		}
}

#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include <boost/optional.hpp>
#include <boost/filesystem/convenience.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>


#include "spl.hpp"
#include "ptable.hpp"
#include "clr_parser_gen.hpp"

//#include "../common/ast.hpp"

namespace splicpp
{
	class spl_parser
	{
		static const char sep = ' ';
	
		const spl g;
		const ptable t;
		
		template <typename T>
		std::vector<T> autoparse_plus(const std::string str, const cst_node n, boost::function<T (const std::string str, const cst_node n)> f) const
		{
			const std::string s_name = g.fetch_symbol(n.fetch_stid(g))->name;
			
			const std::string appendix = "_plus";
			const std::string prefix = s_name.substr(0, s_name.size() - appendix.size());
			
			assert(s_name.substr(s_name.size() - appendix.size()) == appendix);
			
			if(n.size() == 1)
				return std::vector<T> { f(n[0]->as_node()) };
			else if(n.size() == 2)
			{
				std::vector<T> result = autoparse_plus(str, n[0]->as_node(), f);
				result.push_back(f(n[1]->as_node()));
				return result;
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		template <typename T>
		std::vector<T> autoparse_kleene(const std::string str, const cst_node n, boost::function<T (const std::string str, const cst_node n)> f) const
		{
			const std::string s_name = g.fetch_symbol(n.fetch_stid(g))->name;
			
			const std::string appendix = "_kleene";
			const std::string prefix = s_name.substr(0, s_name.size() - appendix.size());
			
			assert(s_name.substr(s_name.size() - appendix.size()) == appendix);
			
			std::vector<T> result;
			if(n.size() == 0)
				return result;
			else if(n.size() == 2)
			{
				result = autoparse_plus(str, n[0]->as_node(), f);
				result.push_back(f(n[1]->as_node()));
				return result;
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		template <typename T>
		boost::optional<T> autoparse_opt(const std::string str, const cst_node n, boost::function<T (const std::string str, const cst_node n)> f) const
		{
			const std::string s_name = g.fetch_symbol(n.fetch_stid(g))->name;
			
			const std::string appendix = "_opt";
			const std::string prefix = s_name.substr(0, s_name.size() - appendix.size());
			
			assert(s_name.substr(s_name.size() - appendix.size()) == appendix);
			
			if(n.size() == 0)
				return boost::optional<T>();
			else if(n.size() == 1)
				return f(n[1]->as_node());
			else
				throw std::logic_error("unexpected rule");
		}
		
		void parse_prog(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_prog");
			
			if(n.size() == 1)
				parse_decl(str, n[0]->as_node());
			else if(n.size() == 2)
			{
				parse_prog(str, n[0]->as_node());
				parse_decl(str, n[1]->as_node());
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		void parse_decl(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_decl");
			
			if(n.size() == 1)
			{
				if(n[0]->is_node(g, "nl_var_decl"))
					parse_var_decl(str, n[0]->as_node());
				else if(n[0]->is_node(g, "nl_fun_decl"))
					parse_fun_decl(str, n[0]->as_node());
				else
					throw std::logic_error("unexpected rule");				
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		void parse_var_decl(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_var_decl");
			assert(n.size() == 5);
			
			parse_type(str, n[0]->as_node());

			n[2]->assert_token(g, "l_assignment");
			std::cout << parse_id(str, n[1]) << sep << "=" << sep;
			
			parse_exp(str, n[3]->as_node());
			n[4]->assert_token(g, "l_semicolon");
			
			std::cout << ';' << std::endl;
		}
		
		void parse_f_args_opt(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_f_args_opt");
			
			if(n.size() == 0)
				return;
			else if(n.size() == 1)
				parse_f_args(str, n[0]->as_node());
			else
				throw std::logic_error("unexpected rule");
		}
		
		void parse_var_decl_kleene(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_var_decl_kleene");
			
			if(n.size() == 0)
				return;
			else if(n.size() == 2)
			{
				parse_var_decl_kleene(str, n[0]->as_node());
				parse_var_decl(str, n[1]->as_node());
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		void parse_stmt_plus(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_stmt_plus");
			
			if(n.size() == 1)
				parse_stmt(str, n[0]->as_node());
			else if(n.size() == 2)
			{
				parse_stmt_plus(str, n[0]->as_node());
				parse_stmt(str, n[1]->as_node());
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		void parse_fun_decl(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_fun_decl");
			assert(n.size() == 9);
			
			parse_ret_type(str, n[0]->as_node());
			std::string id = parse_id(str, n[1]);
			n[2]->assert_token(g, "l_bracket_left");
			
			std::cout << id << '(';
			
			parse_f_args_opt(str, n[3]->as_node());
			n[4]->assert_token(g, "l_bracket_right");
			n[5]->assert_token(g, "l_cbracket_left");
			
			std::cout << ") {";
			
			parse_var_decl_kleene(str, n[6]->as_node());
			parse_stmt_plus(str, n[7]->as_node());
			n[8]->assert_token(g, "l_cbracket_right");
			
			std::cout << "}";
		}
		
		void parse_ret_type(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_ret_type");
			assert(n.size() == 1);
			
			if(n[0]->is_node(g, "nl_type"))
				parse_type(str, n[0]->as_node());
			else if(n[0]->is_token(g, "l_void"))
				std::cout << "Void";
		}
		
		void parse_type(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_type");
			
			if(n.size() == 1)
			{
				if(n[0]->is_token(g, "l_int"))
					std::cout << "Int";
				else if(n[0]->is_token(g, "l_bool"))
					std::cout << "Bool";
				else if(n[0]->is_token(g, "l_id"))
					std::cout << parse_id(str, n[0]);
			}
			else if(n.size() == 3)
			{
				n[0]->assert_token(g, "l_sbracket_left");
				std::cout << '{';
				
				parse_type(str, n[1]->as_node());
				
				n[2]->assert_token(g, "l_sbracket_right");
				std::cout << '}';
			}
			else if(n.size() == 5)
			{
				n[0]->assert_token(g, "l_bracket_left");
				std::cout << '(';
				
				parse_type(str, n[1]->as_node());
				
				n[2]->assert_token(g, "l_comma");
				std::cout << ',';
				
				parse_type(str, n[3]->as_node());
				
				n[4]->assert_token(g, "l_bracket_right");
				std::cout << ')';
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		void parse_f_args(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_f_args");
			
			if(n.size() == 2)
			{
				parse_type(str, n[0]->as_node());
				std::cout << parse_id(str, n[1]);
			}
			else if(n.size() == 4)
			{
				parse_type(str, n[0]->as_node());
				std::cout << parse_id(str, n[1]);

				n[2]->assert_token(g, "l_comma");
				std::cout << ',';
				
				parse_f_args(str, n[3]->as_node());
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		void parse_nested_stmt(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_nested_stmt");
			
			if(n.size() == 1)
				parse_stmt(str, n[0]->as_node());
			else if(n.size() == 2)
			{
				parse_nested_stmt(str, n[0]->as_node());
				parse_stmt(str, n[1]->as_node());
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		void parse_stmt(const std::string str, const cst_node n) const
		{
			//
		}
		
		void parse_fun_call(const std::string str, const cst_node n) const
		{
			//
		}
		
		void parse_act_args(const std::string str, const cst_node n) const
		{
			//
		}
		
		void parse_int(const std::string str, const cst_node n) const
		{
			//
		}
		
		void parse_exp(const std::string str, const cst_node n) const
		{
			//
		}
		
		void parse_exp1(const std::string str, const cst_node n) const
		{
			//
		}
		
		void parse_exp2(const std::string str, const cst_node n) const
		{
			//
		}
		
		void parse_exp3(const std::string str, const cst_node n) const
		{
			//
		}
		
		void parse_exp4(const std::string str, const cst_node n) const
		{
			//
		}
		
		void parse_exp5(const std::string str, const cst_node n) const
		{
			//
		}
		
		void parse_exp6(const std::string str, const cst_node n) const
		{
			//
		}
		
		void parse_exp7(const std::string str, const cst_node n) const
		{
			//
		}
		
		void parse_exp8(const std::string str, const cst_node n) const
		{
			//
		}
		
		std::string parse_id(const std::string str, const std::shared_ptr<cst_element> e) const
		{
			e->assert_token(g, "l_id");
			return e->as_token().as_string(str);
		}
		
	public:
		spl_parser()
		: g()
		, t(fetch_ptable(g))
		{}
		
		static ptable fetch_ptable(const grammar& g)
		{
			const std::string filename = "cache/spl.ptable";
			ptable result;
			
			if(boost::filesystem::exists(filename))
			{
				std::ifstream ifs(filename);
				boost::archive::text_iarchive ia(ifs);
				ia >> result;
			}
			else
			{
				result = splicpp::clr_parser_gen::generate(g, resolve_conflicts);
				
				std::ofstream ofs(filename);
				boost::archive::text_oarchive oa(ofs);
				oa << result;
			}
			
			return result;
		}		
		
		void parse(std::string str) const;
		
		void print_g() const;
		void print_t() const;
		
		static void resolve_conflicts(std::vector<ptable::acttransition>& transitions, const size_t i, const std::vector<itemset<1>> c, const stid a, const grammar g)
		{
			resolve_conflict_id(transitions, i, c, a, g);
			resolve_conflict_dangling_else(transitions, i, c, a, g);
		}

		static void resolve_conflict_id(std::vector<ptable::acttransition>& transitions, const size_t, const std::vector<itemset<1>>, const stid a, const grammar g)
		{
			/*
				Problem: S/R-conflict on rule
					nl_ret_type :== nl_type
			
				On token
					l_id
			
				Solution: choose shift if conflict on rule
			*/
			
			if(transitions.size() != 2)
				return;
			
			if(a != g.fetch_stid("l_id"))
				return;
			
			size_t strans = 0, rtrans = 1;
			if(transitions.at(strans).t == ptable::acttransition::t_shift && transitions.at(rtrans).t == ptable::acttransition::t_shift)
				return; //shift-shift conflict
				
			if(transitions.at(strans).t == ptable::acttransition::t_reduce && transitions.at(rtrans).t == ptable::acttransition::t_shift)
				std::swap(strans, rtrans);
			
			assert(transitions.at(strans).t == ptable::acttransition::t_shift && transitions.at(rtrans).t == ptable::acttransition::t_reduce);
			
			const stid NL_RET_TYPE = g.fetch_stid("nl_ret_type"), NL_TYPE = g.fetch_stid("nl_type");
			const rule r = rule(NL_RET_TYPE) + NL_TYPE;
			
			rule conflict_rule = g.fetch_rule(transitions.at(rtrans).rule);
			
			if(conflict_rule != r)
				return;
			
			transitions.erase(transitions.begin() + strans);
		}

		static void resolve_conflict_dangling_else(std::vector<ptable::acttransition>& transitions, const size_t, const std::vector<itemset<1>>, const stid a, const grammar g)
		{
			/*
				Problem: S/R-conflict on rule
					nl_stmt :== l_if l_bracket_left nl_exp l_bracket_right nl_stmt
				
				To rule
					nl_stmt :== l_if l_bracket_left nl_exp l_bracket_right nl_stmt l_else nl_stmt
				
				Solution: choose shift if conflict on first rule
			*/
			
			if(transitions.size() != 2)
				return;
			
			if(a != g.fetch_stid("l_else"))
				return;
			
			size_t strans = 0, rtrans = 1;
			if(transitions.at(strans).t == ptable::acttransition::t_shift && transitions.at(rtrans).t == ptable::acttransition::t_shift)
				return; //shift-shift conflict
			
			if(transitions.at(strans).t == ptable::acttransition::t_reduce && transitions.at(rtrans).t == ptable::acttransition::t_shift)
				std::swap(strans, rtrans);
			
			assert(transitions.at(strans).t == ptable::acttransition::t_shift && transitions.at(rtrans).t == ptable::acttransition::t_reduce);
			
			const stid NL_STMT = g.fetch_stid("nl_stmt"), L_IF = g.fetch_stid("l_if"), L_BRACKET_LEFT = g.fetch_stid("l_bracket_left"), NL_EXP = g.fetch_stid("nl_exp"), L_BRACKET_RIGHT = g.fetch_stid("l_bracket_right");
			const rule r = rule(NL_STMT) + L_IF + L_BRACKET_LEFT + NL_EXP + L_BRACKET_RIGHT + NL_STMT;
			
			rule conflict_rule = g.fetch_rule(transitions.at(rtrans).rule);
			
			if(conflict_rule != r)
				return;
			
			transitions.erase(transitions.begin() + rtrans);
		}
	};
}

#endif

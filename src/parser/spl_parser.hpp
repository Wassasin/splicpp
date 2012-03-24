#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/convenience.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>

#include "../common/errors.hpp"

#include "spl.hpp"
#include "ptable.hpp"
#include "clr_parser_gen.hpp"

#include "../ast/ast_decl.hpp"

namespace splicpp
{
	class spl_parser
	{
		static const char sep = ' ';
	
		const spl g;
		const ptable t;
		
		template <typename T>
		std::vector<T> autoparse_plus(const std::string str, const cst_node n, boost::function<T (const spl_parser*, const std::string str, const cst_node n)> f) const
		{
			const std::string s_name = g.fetch_symbol(n.fetch_stid(g))->name;
			
			const std::string appendix = "_plus";
			const std::string prefix = s_name.substr(0, s_name.size() - appendix.size());
			
			assert(s_name.substr(s_name.size() - appendix.size()) == appendix);
			
			if(n.size() == 1)
				return std::vector<T> { f(this, str, n[0]->as_node()) };
			else if(n.size() == 2)
			{
				std::vector<T> result = autoparse_plus(str, n[0]->as_node(), f);
				result.push_back(f(this, str, n[1]->as_node()));
				return result;
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		template <typename T>
		std::vector<T> autoparse_kleene(const std::string str, const cst_node n, boost::function<T (const spl_parser*, const std::string str, const cst_node n)> f) const
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
				result = autoparse_kleene(str, n[0]->as_node(), f);
				result.push_back(f(this, str, n[1]->as_node()));
				return result;
			}
			else
				throw std::logic_error("unexpected rule");
		}
		
		template <typename T>
		boost::optional<T> autoparse_opt(const std::string str, const cst_node n, boost::function<T (const spl_parser*, const std::string str, const cst_node n)> f) const
		{
			const std::string s_name = g.fetch_symbol(n.fetch_stid(g))->name;
			
			const std::string appendix = "_opt";
			const std::string prefix = s_name.substr(0, s_name.size() - appendix.size());
			
			assert(s_name.substr(s_name.size() - appendix.size()) == appendix);
			
			if(n.size() == 0)
				return boost::optional<T>();
			else if(n.size() == 1)
				return f(this, str, n[0]->as_node());
			else
				throw std::logic_error("unexpected rule");
		}
		
		std::vector<std::shared_ptr<ast_decl>> parse_prog(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_decl> parse_decl(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_var_decl> parse_var_decl(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_fun_decl> parse_fun_decl(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_type> parse_type(const std::string str, const cst_node n) const;
		std::vector<std::shared_ptr<ast_f_arg>> parse_f_args(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_stmt> parse_stmt(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_fun_call> parse_fun_call(const std::string str, const cst_node n) const;
		std::vector<std::shared_ptr<ast_exp>> parse_act_args(const std::string str, const cst_node n) const;
		
		std::shared_ptr<ast_exp> parse_exp(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_exp> parse_exp1(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_exp> parse_exp2(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_exp> parse_exp3(const std::string str, const cst_node n) const;
		ast_exp_op2::op_type parse_compr_op(const cst_node n) const;
		std::shared_ptr<ast_exp> parse_exp4(const std::string str, const cst_node n) const;
		ast_exp_op2::op_type parse_sum_op(const cst_node n) const;
		std::shared_ptr<ast_exp> parse_exp5(const std::string str, const cst_node n) const;
		ast_exp_op2::op_type parse_product_op(const cst_node n) const;
		std::shared_ptr<ast_exp> parse_exp6(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_exp> parse_exp7(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_exp> parse_exp8(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_exp_int> parse_digit(const std::string str, const cst_node n) const;
		std::shared_ptr<ast_id> parse_id(const std::string str, const std::shared_ptr<cst_element> e) const;
		
		parse_error unexpected_element(const std::shared_ptr<cst_element> e) const;
		parse_error unexpected_token(const token t) const;
		parse_error unexpected_node(const cst_node n) const;
		
	public:
		spl_parser()
		: g()
		, t(fetch_ptable(g))
		{}	
		
		std::vector<std::shared_ptr<ast_decl>> parse(std::string str) const;
		
		void print_g() const;
		void print_t() const;
		
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
		
		static void resolve_conflicts(std::vector<ptable::acttransition>& transitions, const size_t i, const std::vector<itemset<1>> c, const stid a, const grammar g)
		{
			resolve_conflict_dangling_else(transitions, i, c, a, g);
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

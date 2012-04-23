#include "spl_parser.hpp"

#include "../common/sloc.hpp"

#include "ptable.hpp"
#include "lexer.hpp"
#include "clr_parser_gen.hpp"

#include "../ast/ast_decl.hpp"
#include "../ast/ast_type.hpp"
#include "../ast/ast_stmt.hpp"
#include "../ast/ast_fun_call.hpp"
#include "../ast/ast_var_decl.hpp"
#include "../ast/ast_fun_decl.hpp"
#include "../ast/ast_f_arg.hpp"
#include "../ast/ast_id.hpp"

namespace splicpp
{
		std::shared_ptr<ast_prog> spl_parser::parse(std::string str) const
		{
			lexer l(g, str);
			cst_element e = t.parse(l);
			
			return parse_prog(str, e.as_node()[0]->as_node());
		}
		
		void spl_parser::print_g() const
		{
			g.print();
		}
		
		void spl_parser::print_t() const
		{
			t.print(g);
		}
		
		ptable spl_parser::fetch_ptable(const grammar& g)
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
		
		
		std::shared_ptr<ast_prog> spl_parser::parse_prog(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_prog");
			
			std::shared_ptr<ast_prog> result(new ast_prog(n.sl()));
			for(auto decl : autoparse_plus<std::shared_ptr<ast_decl>>(str, n[0]->as_node(), &spl_parser::parse_decl))
				result->add_decl(decl);
			
			return result;
		}
		
		std::shared_ptr<ast_decl> spl_parser::parse_decl(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_decl");
			
			if(n.size() == 1)
			{
				if(n[0]->is_node(g, "nl_var_decl"))
					return std::shared_ptr<ast_decl>(new ast_decl_var(parse_var_decl(str, n[0]->as_node()), n.sl()));
				else if(n[0]->is_node(g, "nl_fun_decl"))
					return std::shared_ptr<ast_decl>(new ast_decl_fun(parse_fun_decl(str, n[0]->as_node()), n.sl()));
				else
					throw parse_error("unexpected rule");				
			}
			else
				throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_var_decl> spl_parser::parse_var_decl(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_var_decl");
			assert(n.size() == 5);
			
			n[2]->assert_token(g, "l_assignment");
			n[4]->assert_token(g, "l_semicolon");
			
			return std::shared_ptr<ast_var_decl>(
				new ast_var_decl(
					parse_type(str, n[0]->as_node()),
					parse_id(str, n[1]),
					parse_exp(str, n[3]->as_node()),
					n.sl()
				)
			);
		}
		
		std::shared_ptr<ast_fun_decl> spl_parser::parse_fun_decl(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_fun_decl");
			assert(n.size() == 9);
			
			n[2]->assert_token(g, "l_bracket_left");			
			n[4]->assert_token(g, "l_bracket_right");
			n[5]->assert_token(g, "l_cbracket_left");
			n[8]->assert_token(g, "l_cbracket_right");
			
			std::shared_ptr<ast_type> rtype;
			if(n[0]->is_node(g, "nl_type"))
				rtype = parse_type(str, n[0]->as_node());
			else if(n[0]->is_token(g, "l_void"))
				rtype = std::shared_ptr<ast_type>(new ast_type_void(n[0]->as_token().as_sloc()));
			else
				throw unexpected_element(n[0]);
			
			auto id = parse_id(str, n[1]);
			std::shared_ptr<ast_fun_decl> result(new ast_fun_decl(rtype, id, n.sl()));
			
			auto args = autoparse_opt<std::vector<std::shared_ptr<ast_f_arg>>>(str, n[3]->as_node(), &spl_parser::parse_f_args);
			auto decls = autoparse_kleene<std::shared_ptr<ast_var_decl>>(str, n[6]->as_node(), &spl_parser::parse_var_decl);
			auto stmts = autoparse_plus<std::shared_ptr<ast_stmt>>(str, n[7]->as_node(), &spl_parser::parse_stmt);
			
			if(args)
				for(auto arg : args.get())
					result->add_arg(arg);
			
			for(auto decl : decls)
				result->add_decl(decl);
			
			for(auto stmt : stmts)
				result->add_stmt(stmt);
			
			return result;
		}
		
		std::shared_ptr<ast_type> spl_parser::parse_type(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_type");
			
			std::shared_ptr<ast_type> result;
			if(n.size() == 1)
			{
				if(n[0]->is_token(g, "l_int"))
					return std::shared_ptr<ast_type>(new ast_type_int(n.sl()));
				else if(n[0]->is_token(g, "l_bool"))
					return std::shared_ptr<ast_type>(new ast_type_bool(n.sl()));
				else if(n[0]->is_token(g, "l_id"))
					return std::shared_ptr<ast_type>(new ast_type_id(parse_id(str, n[0]), n.sl()));
				else
					throw unexpected_element(n[0]);
			}
			else if(n.size() == 3)
			{
				n[0]->assert_token(g, "l_sbracket_left");
				n[2]->assert_token(g, "l_sbracket_right");
				
				return std::shared_ptr<ast_type>(new ast_type_array(parse_type(str, n[1]->as_node()), n.sl()));
			}
			else if(n.size() == 5)
			{
				n[0]->assert_token(g, "l_bracket_left");
				n[2]->assert_token(g, "l_comma");
				n[4]->assert_token(g, "l_bracket_right");
				
				return std::shared_ptr<ast_type>(new ast_type_tuple(parse_type(str, n[1]->as_node()), parse_type(str, n[3]->as_node()), n.sl()));
			}
			else
				throw parse_error("unexpected rule");
			
			return result;
		}
		
		std::vector<std::shared_ptr<ast_f_arg>> spl_parser::parse_f_args(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_f_args");
			
			if(n.size() == 2)
				return create_vector_ptr<ast_f_arg>(std::shared_ptr<ast_f_arg>(new ast_f_arg(parse_type(str, n[0]->as_node()), parse_id(str, n[1]), n.sl())));
			else if(n.size() == 4)
			{
				n[1]->assert_token(g, "l_comma");
			
				auto args = parse_f_args(str, n[0]->as_node());
				std::shared_ptr<ast_f_arg> arg(new ast_f_arg(parse_type(str, n[2]->as_node()), parse_id(str, n[3]), n.sl()));
				
				args.push_back(arg);
				return args;
			}
			else
				throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_stmt> spl_parser::parse_stmt(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_stmt");
			
			if(n.size() == 2)
			{
				n[1]->assert_token(g, "l_semicolon");
				return std::shared_ptr<ast_stmt>(new ast_stmt_fun_call(parse_fun_call(str, n[0]->as_node()), n.sl()));
			}
			else if(n.size() == 3)
			{
				if(n[0]->is_token(g, "l_cbracket_left"))
				{
					n[0]->assert_token(g, "l_cbracket_left");
					n[2]->assert_token(g, "l_cbracket_right");
				
					std::shared_ptr<ast_stmt_stmts> result(new ast_stmt_stmts(n.sl()));
					for(auto stmt : autoparse_kleene<std::shared_ptr<ast_stmt>>(str, n[1]->as_node(), &spl_parser::parse_stmt))
						result->add_stmt(stmt);
						
					return std::static_pointer_cast<ast_stmt>(result);
				}
				else if(n[0]->is_token(g, "l_return"))
				{
					n[0]->assert_token(g, "l_return");
					n[2]->assert_token(g, "l_semicolon");
					
					auto opt_decl = autoparse_opt<std::shared_ptr<ast_exp>>(str, n[1]->as_node(), &spl_parser::parse_exp);
					if(opt_decl)
						return std::shared_ptr<ast_stmt>(new ast_stmt_return(opt_decl.get(), n.sl()));
					else
						return std::shared_ptr<ast_stmt>(new ast_stmt_return(n.sl()));
				}
				else
					throw unexpected_element(n[0]);
			}
			else if(n.size() == 4)
			{
				n[0]->assert_token(g, "l_id");
				n[1]->assert_token(g, "l_assignment");
				n[3]->assert_token(g, "l_semicolon");
				
				return std::shared_ptr<ast_stmt>(new ast_stmt_assignment(parse_id(str, n[0]), parse_exp(str, n[2]->as_node()), n.sl()));
			}
			else if(n.size() == 5)
			{
				if(n[0]->is_token(g, "l_while"))
				{
					n[0]->assert_token(g, "l_while");
					n[1]->assert_token(g, "l_bracket_left");
					n[3]->assert_token(g, "l_bracket_right");
					
					return std::shared_ptr<ast_stmt>(new ast_stmt_while(parse_exp(str, n[2]->as_node()), parse_stmt(str, n[4]->as_node()), n.sl()));
				}
				else if(n[0]->is_token(g, "l_if"))
				{
					n[0]->assert_token(g, "l_if");
					n[1]->assert_token(g, "l_bracket_left");
					n[3]->assert_token(g, "l_bracket_right");
					
					return std::shared_ptr<ast_stmt>(new ast_stmt_if(parse_exp(str, n[2]->as_node()), parse_stmt(str, n[4]->as_node()), n.sl()));
				}
				else
					throw parse_error("unexpected rule");
			}
			else if(n.size() == 7)
			{
				n[0]->assert_token(g, "l_if");
				n[1]->assert_token(g, "l_bracket_left");
				n[3]->assert_token(g, "l_bracket_right");
				n[5]->assert_token(g, "l_else");
				
				return std::shared_ptr<ast_stmt>(new ast_stmt_if(parse_exp(str, n[2]->as_node()), parse_stmt(str, n[4]->as_node()), parse_stmt(str, n[6]->as_node()), n.sl()));
			}
			else
					throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_fun_call> spl_parser::parse_fun_call(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_fun_call");
			
			if(n.size() == 3)
			{
				n[1]->assert_token(g, "l_bracket_left");
				n[2]->assert_token(g, "l_bracket_right");
				
				return std::shared_ptr<ast_fun_call>(new ast_fun_call(parse_id(str, n[0]), n.sl()));
			}
			if(n.size() == 4)
			{
				n[1]->assert_token(g, "l_bracket_left");
				n[3]->assert_token(g, "l_bracket_right");
			
				std::shared_ptr<ast_fun_call> f(new ast_fun_call(parse_id(str, n[0]), n.sl()));
				for(auto arg : parse_act_args(str, n[2]->as_node()))
					f->add_arg(arg);
				
				return f;
			}
			else
				throw parse_error("unexpected rule");
		}
		
		std::vector<std::shared_ptr<ast_exp>> spl_parser::parse_act_args(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_act_args");
			
			if(n.size() == 1)
			{
				return create_vector_ptr<ast_exp>(parse_exp(str, n[0]->as_node()));
			}
			else if(n.size() == 3)
			{
				auto args = parse_act_args(str, n[0]->as_node());
				n[1]->assert_token(g, "l_comma");
				args.push_back(parse_exp(str, n[2]->as_node()));
				return args;
			}
			else
				throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_exp> spl_parser::parse_exp(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_exp");
			return parse_exp1(str, n[0]->as_node());
		}
		
		std::shared_ptr<ast_exp> spl_parser::parse_exp1(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_exp1");
			if(n.size() == 1)
				return parse_exp2(str, n[0]->as_node());
			else if(n.size() == 3)
			{
				n[1]->assert_token(g, "l_disjunction");
				return std::shared_ptr<ast_exp>(new ast_exp_op2(parse_exp2(str, n[0]->as_node()), ast_exp_op2::op_type::op_disjunction, parse_exp1(str, n[2]->as_node()), n.sl()));
			}
			else
				throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_exp> spl_parser::parse_exp2(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_exp2");
			if(n.size() == 1)
				return parse_exp3(str, n[0]->as_node());
			else if(n.size() == 3)
			{
				n[1]->assert_token(g, "l_conjunction");
				return std::shared_ptr<ast_exp>(new ast_exp_op2(parse_exp3(str, n[0]->as_node()), ast_exp_op2::op_type::op_conjunction, parse_exp2(str, n[2]->as_node()), n.sl()));
			}
			else
				throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_exp> spl_parser::parse_exp3(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_exp3");
			if(n.size() == 1)
				return parse_exp4(str, n[0]->as_node());
			else if(n.size() == 3)
				return std::shared_ptr<ast_exp>(new ast_exp_op2(parse_exp3(str, n[0]->as_node()), parse_compr_op(n[1]->as_node()), parse_exp4(str, n[2]->as_node()), n.sl()));
			else
				throw parse_error("unexpected rule");
		}
		
		ast_exp_op2::op_type spl_parser::parse_compr_op(const cst_node n) const
		{
			n.assert_stid(g, "nl_compr_op");
			
			const stid s = n[0]->as_token().type;
			if(s == g.L_LEQ)
				return ast_exp_op2::op_leq;
			else if(s == g.L_GEQ)
				return ast_exp_op2::op_geq;
			else if(s == g.L_EQ)
				return ast_exp_op2::op_eq;
			else if(s == g.L_NEQ)
				return ast_exp_op2::op_neq;
			else if(s == g.L_LESSER)
				return ast_exp_op2::op_lesser;
			else if(s == g.L_GREATER)
				return ast_exp_op2::op_greater;
			
			throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_exp> spl_parser::parse_exp4(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_exp4");
			if(n.size() == 1)
				return parse_exp5(str, n[0]->as_node());
			else if(n.size() == 3)
				return std::shared_ptr<ast_exp>(new ast_exp_op2(parse_exp4(str, n[0]->as_node()), parse_sum_op(n[1]->as_node()), parse_exp5(str, n[2]->as_node()), n.sl()));
			else
				throw parse_error("unexpected rule");
		}
		
		ast_exp_op2::op_type spl_parser::parse_sum_op(const cst_node n) const
		{
			n.assert_stid(g, "nl_sum_op");
			
			const stid s = n[0]->as_token().type;
			if(s == g.L_PLUS)
				return ast_exp_op2::op_plus;
			else if(s == g.L_MINUS)
				return ast_exp_op2::op_minus;
			
			throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_exp> spl_parser::parse_exp5(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_exp5");
			if(n.size() == 1)
				return parse_exp6(str, n[0]->as_node());
			else if(n.size() == 3)
				return std::shared_ptr<ast_exp>(new ast_exp_op2(parse_exp5(str, n[0]->as_node()), parse_product_op(n[1]->as_node()), parse_exp6(str, n[2]->as_node()), n.sl()));
			else
				throw parse_error("unexpected rule");
		}
		
		ast_exp_op2::op_type spl_parser::parse_product_op(const cst_node n) const
		{
			n.assert_stid(g, "nl_product_op");
			
			const stid s = n[0]->as_token().type;
			if(s == g.L_TIMES)
				return ast_exp_op2::op_times;
			else if(s == g.L_DIVIDES)
				return ast_exp_op2::op_divides;
			else if(s == g.L_MOD)
				return ast_exp_op2::op_mod;
					
			throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_exp> spl_parser::parse_exp6(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_exp6");
			if(n.size() == 1)
				return parse_exp7(str, n[0]->as_node());
			else if(n.size() == 2)
			{
				n[0]->assert_token(g, "l_negation");
				return std::shared_ptr<ast_exp>(new ast_exp_negation(parse_exp6(str, n[1]->as_node()), n.sl()));
			}
			else
				throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_exp> spl_parser::parse_exp7(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_exp7");
			if(n.size() == 1)
				return parse_exp8(str, n[0]->as_node());
			else if(n.size() == 3)
			{
				n[1]->assert_token(g, "l_colon");
				return std::shared_ptr<ast_exp>(new ast_exp_op2(parse_exp8(str, n[0]->as_node()), ast_exp_op2::op_cons, parse_exp7(str, n[2]->as_node()), n.sl()));
			}
			else
				throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_exp> spl_parser::parse_exp8(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_exp8");
			
			if(n.size() == 1)
			{
				if(n[0]->is_node(g, "nl_fun_call"))
					return std::shared_ptr<ast_exp>(new ast_exp_fun_call(parse_fun_call(str, n[0]->as_node()), n.sl()));
				else if(n[0]->is_node(g, "nl_digit"))
					return parse_digit(str, n[0]->as_node());
				else if(n[0]->is_token(g, "l_id"))
					return std::shared_ptr<ast_exp>(new ast_exp_id(parse_id(str, n[0]), n.sl()));
				else if(n[0]->is_token(g, "l_true"))
					return std::shared_ptr<ast_exp>(new ast_exp_bool(true, n.sl()));
				else if(n[0]->is_token(g, "l_false"))
					return std::shared_ptr<ast_exp>(new ast_exp_bool(false, n.sl()));
				else
					throw parse_error("unexpected rule");
			}
			else if(n.size() == 2)
			{
				n[0]->assert_token(g, "l_sbracket_left");
				n[1]->assert_token(g, "l_sbracket_right");
				
				return std::shared_ptr<ast_exp>(new ast_exp_nil(n.sl()));
			}
			else if(n.size() == 3)
			{
				n[0]->assert_token(g, "l_bracket_left");
				n[2]->assert_token(g, "l_bracket_right");
				
				return parse_exp(str, n[1]->as_node());
			}
			else if(n.size() == 5)
			{
				n[0]->assert_token(g, "l_bracket_left");
				n[2]->assert_token(g, "l_comma");
				n[4]->assert_token(g, "l_bracket_right");
				
				return std::shared_ptr<ast_exp>(new ast_exp_tuple(parse_exp(str, n[1]->as_node()), parse_exp(str, n[3]->as_node()), n.sl()));
			}
			else
				throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_exp_int> spl_parser::parse_digit(const std::string str, const cst_node n) const
		{
			n.assert_stid(g, "nl_digit");
			
			if(n.size() == 1)
			{
				n[0]->assert_token(g, "l_digit");
				const token t = n[0]->as_token();
				return std::shared_ptr<ast_exp_int>(new ast_exp_int(boost::lexical_cast<int>(t.as_string(str)), t.as_sloc()));
			}
			else if(n.size() == 2)
			{
				n[0]->assert_token(g, "l_minus");
				n[1]->assert_token(g, "l_digit");
				return std::shared_ptr<ast_exp_int>(new ast_exp_int(-1 * boost::lexical_cast<int>(n[1]->as_token().as_string(str)), n.sl()));
			}
			else
				throw parse_error("unexpected rule");
		}
		
		std::shared_ptr<ast_id> spl_parser::parse_id(const std::string str, const std::shared_ptr<cst_element> e) const
		{
			e->assert_token(g, "l_id");
			const token t = e->as_token();
			
			std::shared_ptr<ast_id> result(new ast_id(t.as_string(str), t.as_sloc()));
			return result;
		}
		
		parse_error spl_parser::unexpected_element(const std::shared_ptr<cst_element> e) const
		{
			if(e->is_node())
				return unexpected_node(e->as_node());
			else
				return unexpected_token(e->as_token());
		}
		
		parse_error spl_parser::unexpected_token(const token t) const
		{
			std::stringstream s;
			s << "unexpected token: " << g.fetch_symbol(t.type)->name;
			return parse_error(s.str());
		}
		
		parse_error spl_parser::unexpected_node(const cst_node n) const
		{
			std::stringstream s;
			s << "unexpected node: " << g.fetch_symbol(n.fetch_stid(g))->name;
			return parse_error(s.str());
		}
}

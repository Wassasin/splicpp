#include "ast_fun_decl.hpp"

#include "../typing/typecontext.hpp"
#include "../typing/ltypecontext.hpp"

#include "../typing/varcontext.hpp"
#include "../typing/symboltable.hpp"

#include "../typing/types/sl_polytype.hpp"
#include "../typing/types/sl_type.hpp"
#include "../typing/types/sl_type_function.hpp"
#include "../typing/types/sl_type_unbound.hpp"
#include "../typing/types/sl_type_void.hpp"

#include "ast_id.hpp"
#include "ast_f_arg.hpp"
#include "ast_stmt.hpp"

#include "../ir/ircontext.hpp"
#include "../ir/ir_exp_binop.hpp"
#include "../ir/ir_exp_const.hpp"
#include "../ir/ir_exp_mem.hpp"
#include "../ir/ir_exp_temp.hpp"
#include "../ir/ir_stmt_jump.hpp"
#include "../ir/ir_stmt_label.hpp"
#include "../ir/ir_stmt_move.hpp"

namespace splicpp
{
	void ast_fun_decl::assign(sid i)
	{
		id->assign(i);
	}
	
	std::string ast_fun_decl::fetch_name() const
	{
		return id->fetch_name();
	}
	
	sid ast_fun_decl::fetch_id() const
	{
		return id->fetch_id();
	}
	
	void ast_fun_decl::register_locals(symboltable& s, varcontext& c)
	{
		varcontext cvar = c;
		varcontext ctype; //Fresh context
		
		ast_type::register_type(t, s, ctype);
		
		for(auto arg : args)
		{
			cvar.assign(arg->fetch_name(), s.reg_arg(arg));
			arg->register_types(s, ctype);
		}
		
		for(auto decl : decls)
		{
			decl->assign_ids(cvar);
			cvar.assign(decl->fetch_name(), s.reg_lvar(decl));
			decl->register_types(s, ctype);
		}
		
		for(auto stmt : stmts)
			stmt->assign_ids(cvar);
	}
	
	s_ptr<const sl_type> ast_fun_decl::fetch_assigned_type(const typecontext& c) const
	{
		std::vector<s_ptr<const sl_type>> t_args;
		for(const auto arg : args)
			t_args.push_back(arg->fetch_assigned_type(c));
		
		return make_s<const sl_type_function>(t_args, this->t->fetch_type(c), sl);
	}
	
	substitution ast_fun_decl::declare_type(ltypecontext& c) const
	{
		ltypecontext clocal = c;
	
		s_ptr<const sl_type_unbound> r = c.create_fresh(sl);
		std::vector<s_ptr<const sl_type>> t_args;
		for(size_t i = 0; i < args.size(); i++)
			t_args.push_back(args[i]->declare_type(clocal));
		
		s_ptr<const sl_type_function> ft(make_s<const sl_type_function>(t_args, r, sl));
		
		substitution s;
		for(const auto decl : decls)
		{
			clocal = clocal.apply(s);
			s = decl->declare_type(clocal).composite(s);
		}
		
		bool contains_return = false;
		for(const auto stmt : stmts)
		{
			if(stmt->contains_return())
				contains_return = true;
			
			s = stmt->infer_type(clocal.apply(s), r->apply(s)).composite(s);
		}
		
		if(!contains_return)
			s = r->apply(s)->unify(make_s<const sl_type_void>(sl)).composite(s);
		
		c.register_type(id->fetch_id(), sl_polytype::qualify(c.apply(s), ft->apply(s)));
		return s;
	}
	
	s_ptr<const ir_stmt> ast_fun_decl::translate(const ir_label l_function, const ircontext& c) const
	{
		ircontext ccopy = c;
		
		const s_ptr<const ir_exp> r_frame(make_s<ir_exp_temp>(c.frame_reg));
	
		for(size_t i = 0; i < args.size(); i++)
			ccopy.register_memloc(args[i]->fetch_id(), make_s<ir_exp_binop>(
				ir_exp_binop::op_minus,
				r_frame,
				make_s<ir_exp_const>((int)(i + 2)) //[FP + 2 + i], where 2 = offset for old FP and return address
			));
		
		for(size_t i = 0; i < decls.size(); i++)
			ccopy.register_memloc(decls[i]->fetch_id(), make_s<ir_exp_binop>(
				ir_exp_binop::op_minus,
				r_frame,
				make_s<ir_exp_const>((int)(i + args.size() + 2)) //[FP + args + 2 + i], where 2 = offset for old FP and return address
			));
		
		s_ptr<const ir_stmt> r(make_s<ir_stmt_label>(l_function));
		
		for(const auto decl : decls)
			ir_stmt::cat(r, decl->translate(ccopy));
		
		for(const auto stmt : stmts)
			ir_stmt::cat(r, stmt->translate(ccopy));
		
		//Always add a return void-statement; just to be sure. May be removed later in the IR-tree.
		ir_stmt::cat(r, make_s<ir_stmt_jump>(
			make_s<ir_exp_mem>(
				make_s<ir_exp_binop>(
					ir_exp_binop::op_minus,
					make_s<ir_exp_temp>(c.frame_reg),
					make_s<ir_exp_const>(1)
				)
			)
		));
		
		return r;
	}
	
	void ast_fun_decl::pretty_print(std::ostream& s, const uint tab) const
	{
		t->pretty_print(s, tab);
		
		s << ' ';
		id->pretty_print(s, tab);
		s << '(';
		
		bool first = true;
		for(const auto arg : args)
		{
			if(first)
				first = false;
			else
				s << ", ";
			
			arg->pretty_print(s, tab);
		}
		
		s << ')';
		ast::print_newline(s, tab);
		s << '{';
		
		for(const auto decl : decls)
		{
			ast::print_newline(s, tab+1);
			decl->pretty_print(s, tab+1);
		}
		
		for(const auto stmt : stmts)
		{
			ast::print_newline(s, tab+1);
			stmt->pretty_print(s, tab+1);
		}
		
		ast::print_newline(s, tab);
		s << '}';
	}
}

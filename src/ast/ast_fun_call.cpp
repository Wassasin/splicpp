#include "ast_fun_call.hpp"

#include "ast_id.hpp"
#include "ast_exp.hpp"

#include "../typing/typecontext.hpp"
#include "../typing/types/sl_type.hpp"
#include "../typing/types/sl_type_function.hpp"
#include "../typing/types/sl_type_unbound.hpp"

#include "../ir/ircontext.hpp"
#include "../ir/ir_exp_mem.hpp"
#include "../ir/ir_exp_name.hpp"
#include "../ir/ir_exp_temp.hpp"
#include "../ir/ir_stmt_call.hpp"
#include "../ir/ir_stmt_label.hpp"
#include "../ir/ir_stmt_move.hpp"

namespace splicpp
{
	void ast_fun_call::assign_ids(const varcontext& c)
	{
		id->assign_ids(c);
		for(auto arg : args)
			arg->assign_ids(c);
	}
	
	substitution ast_fun_call::infer_type(const typecontext& c, const s_ptr<const sl_type> t) const
	{
		std::vector<s_ptr<const sl_type>> targs;
		for(size_t i = 0; i < args.size(); i++)
			targs.push_back(std::static_pointer_cast<const sl_type>(c.create_fresh(sl)));
		
		s_ptr<const sl_type_function> ft(make_s<sl_type_function>(targs, t, sl));
		substitution s = id->infer_type(c, ft);
		
		for(size_t i = 0; i < args.size(); i++)
			s = args[i]->infer_type(c.apply(s), targs[i]->apply(s)).composite(s);
		
		return s;
	}
	
	s_ptr<const ir_stmt> ast_fun_call::translate(const ir_temp return_reg, const ircontext& c) const
	{
		/*
		OLD IMPLEMENTATION; PRESERVED FOR UNDERSTANDING
		
		const ir_label l_return = c.create_label();
		
		const s_ptr<const ir_exp> r_temp(ir_exp_temp::create(c.create_temporary()));
		const s_ptr<const ir_exp> r_frame(ir_exp_temp::create(c.frame_reg));
		const s_ptr<const ir_exp> r_stack(ir_exp_temp::create(c.stack_reg));
		const s_ptr<const ir_exp> r_return(ir_exp_temp::create(return_reg));
		const s_ptr<const ir_exp> r_tmp_return(ir_exp_temp::create(c.return_reg));
		
		//Function call stack items
		std::vector<s_ptr<const ir_exp>> items;
		items.push_back(ir_exp_temp::create(c.frame_reg));
		items.push_back(ir_exp_name::create(l_return));
		for(const auto arg : args)
			items.push_back(arg->translate(c));
		
		//Prepare for function call, and call
		s_ptr<const ir_stmt> r(ir_stmt_move::create(r_temp, r_stack));
		ir_stmt::cat(r, ir_stmt::push(items, c));
		ir_stmt::cat(r, ir_stmt_move::create(r_frame, r_temp)); //Set old stack_ptr as frame_ptr
		ir_stmt::cat(r, ir_stmt_call::create(c.fetch_memloc(id->fetch_id())));
		
		//Clean up after function has been called
		ir_stmt::cat(r, ir_stmt_label::create(l_return));
		ir_stmt::cat(r, ir_stmt_move::create(r_return, r_tmp_return));
		ir_stmt::cat(r, ir_stmt_move::create(r_stack, r_frame)); //Clean up all deprecated entries on stack
		ir_stmt::cat(r, ir_stmt_move::create(r_frame, ir_exp_mem::create(r_frame))); //Set the old frame_ptr back
		*/
		
		const s_ptr<const ir_exp> r_return(ir_exp_temp::create(return_reg));
		const s_ptr<const ir_exp> r_tmp_return(ir_exp_temp::create(c.return_reg));
		
		std::vector<s_ptr<const ir_exp>> items;
		for(const auto arg : args)
			items.push_back(arg->translate(c));
		
		s_ptr<const ir_stmt> r(ir_stmt_call::create(c.fetch_memloc(id->fetch_id()), items));
		ir_stmt::cat(r, ir_stmt_move::create(r_return, r_tmp_return));
		
		return r;
	}

	void ast_fun_call::pretty_print(std::ostream& s, const uint tab) const
	{
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
	}
}

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
		const s_ptr<const ir_exp> r_return(make_s<ir_exp_temp>(return_reg));
		const s_ptr<const ir_exp> r_tmp_return(make_s<ir_exp_temp>(c.return_reg));
		
		std::vector<s_ptr<const ir_exp>> items;
		for(const auto arg : args)
			items.push_back(arg->translate(c));
		
		s_ptr<const ir_stmt> r(make_s<ir_stmt_call>(c.fetch_memloc(id->fetch_id()), items));
		ir_stmt::cat(r, make_s<ir_stmt_move>(r_return, r_tmp_return));
		
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

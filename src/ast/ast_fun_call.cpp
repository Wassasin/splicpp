#include "ast_fun_call.hpp"

#include "ast_id.hpp"
#include "ast_exp.hpp"

#include "../typing/typecontext.hpp"
#include "../typing/types/sl_type.hpp"
#include "../typing/types/sl_type_function.hpp"
#include "../typing/types/sl_type_unbound.hpp"

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
		
		s_ptr<const sl_type_function> ft(new sl_type_function(targs, t, sl));
		substitution s = id->infer_type(c, ft);
		
		for(size_t i = 0; i < args.size(); i++)
			s = args[i]->infer_type(c.apply(s), targs[i]->apply(s)).composite(s);
		
		return s;
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

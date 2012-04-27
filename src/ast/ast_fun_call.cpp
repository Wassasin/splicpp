#include "ast_fun_call.hpp"

#include "ast_id.hpp"
#include "ast_exp.hpp"

#include "../typing/typecontext.hpp"
#include "../typing/types/sl_type.hpp"
#include "../typing/types/sl_type_function.hpp"
#include "../typing/types/sl_type_unbound.hpp"

namespace splicpp
{
	void ast_fun_call::add_arg(s_ptr<ast_exp> exp)
	{
		args.push_back(exp);
	}
	
	void ast_fun_call::assign_ids(const varcontext& c)
	{
		id->assign_ids(c);
		for(auto arg : args)
			arg->assign_ids(c);
	}
	
	substitution ast_fun_call::infer_type(const typecontext& c, const cs_ptr<sl_type> t) const
	{
		std::vector<cs_ptr<sl_type>> targs;
		for(size_t i = 0; i < args.size(); i++)
			targs.push_back(std::static_pointer_cast<const sl_type>(c.create_fresh(sl)));
		
		cs_ptr<sl_type_function> ft(new sl_type_function(targs, t, sl));
		substitution s = id->infer_type(c, ft);
		
		for(size_t i = 0; i < args.size(); i++)
			s = args[i]->infer_type(c.apply(s), targs[i]->apply(s)).composite(s);
		
		std::cout << std::endl;
		std::cout << std::endl << "ast_fun_call::infer_type s: ";
		s.print(std::cout);
		
		std::cout << std::endl << "ast_fun_call::infer_type t: ";
		t->print(std::cout);
		
		std::cout << std::endl << "ast_fun_call::infer_type ft: ";
		ft->print(std::cout);
		
		std::cout << std::endl;
		
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

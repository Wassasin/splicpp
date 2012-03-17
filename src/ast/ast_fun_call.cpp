#include "ast_fun_call.hpp"

namespace splicpp
{
	void ast_fun_call::add_arg(std::shared_ptr<ast_exp> exp)
	{
		args.push_back(exp);
	}

	void ast_fun_call::pretty_print(std::ostream& s) const
	{
		id->pretty_print(s);
		s << '(';
		
		bool first = true;
		for(const auto arg : args)
		{
			if(first)
				first = false;
			else
				s << ", ";
			
			arg->pretty_print(s);
		}
		s << ')';
	}
}

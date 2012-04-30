#include "ir_exp_call.hpp"

#include <sstream>
#include "../common/utils.hpp"

#include "ir_stmt.hpp"

namespace splicpp
{
	void ir_exp_call::print(std::ostream& s, const uint tab) const
	{
		s << "CALL(";
		
		print_newline(s, tab+1);
		func->print(s, tab+1);
		
		s << ",";
		print_newline(s, tab+1);
		s << "{";
		
		delim_printer p(",", s);
		for(const auto arg : args)
		{
			std::stringstream str;
			print_newline(str, tab+2);
			arg->print(str, tab+2);
			p.print(str.str());
		}
		
		print_newline(s, tab+1);
		s << "}";
		print_newline(s, tab);
		s << ")";
	}
}

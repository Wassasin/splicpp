#include "ir_stmt_call.hpp"

#include <sstream>

#include "../common/utils.hpp"

#include "ir_exp.hpp"

#include "../mappers/generic/ir_stmt_mapper.hpp"

namespace splicpp
{
	void ir_stmt_call::map(ir_stmt_mapper& t) const
	{
		t.map(std::static_pointer_cast<const ir_stmt_call>(shared_from_this()));
	}
	
	void ir_stmt_call::print(std::ostream& s, const uint tab) const
	{
		s << "CALL(";
		
		print_newline(s, tab+1);
		e->print(s, tab+1);
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

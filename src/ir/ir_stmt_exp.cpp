#include "ir_stmt_exp.hpp"

#include "ir_exp.hpp"

namespace splicpp
{
	void ir_stmt_exp::print(std::ostream& s, const uint tab) const
	{
		s << "EXP(";
		
		print_newline(s, tab+1);
		e->print(s, tab+1);
				
		print_newline(s, tab);
		s << ")";
	}
}

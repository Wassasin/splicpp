#include "ir_stmt_jump.hpp"

#include "ir_exp.hpp"

namespace splicpp
{
	void ir_stmt_jump::print(std::ostream& s, const uint tab) const
	{
		s << "JUMP(";
		
		print_newline(s, tab+1);
		e->print(s, tab+1);
				
		print_newline(s, tab);
		s << ")";
	}
}

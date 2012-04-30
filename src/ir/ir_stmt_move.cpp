#include "ir_stmt_move.hpp"

#include "ir_exp.hpp"

namespace splicpp
{
	void ir_stmt_move::print(std::ostream& s, const uint tab) const
	{
		s << "MOVE(";
		
		print_newline(s, tab+1);
		e_left->print(s, tab+1);
		s << ",";
		
		print_newline(s, tab+1);
		e_right->print(s, tab+1);
			
		print_newline(s, tab);
		s << ")";
	}
}

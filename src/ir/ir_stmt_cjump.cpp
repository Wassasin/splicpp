#include "ir_stmt_cjump.hpp"

#include "ir_exp.hpp"

namespace splicpp
{
	void ir_stmt_cjump::print(std::ostream& s, const uint tab) const
	{
		s << "CJUMP(";
		
		print_newline(s, tab+1);
		s << r << ",";
		
		print_newline(s, tab+1);
		e_left->print(s, tab+1);
		s << ",";
		
		print_newline(s, tab+1);
		e_right->print(s, tab+1);
		s << ",";
		
		print_newline(s, tab+1);
		s << l_left << ",";
		
		print_newline(s, tab+1);
		s << l_right;
				
		print_newline(s, tab);
		s << ")";
	}
}

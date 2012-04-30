#include "ir_stmt_seq.hpp"

namespace splicpp
{
	void ir_stmt_seq::print(std::ostream& s, const uint tab) const
	{
		s << "SEQ(";
		
		print_newline(s, tab+1);
		s_left->print(s, tab+1);
		s << ",";
		
		print_newline(s, tab+1);
		s_right->print(s, tab+1);
			
		print_newline(s, tab);
		s << ")";
	}
}

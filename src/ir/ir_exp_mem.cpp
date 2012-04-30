#include "ir_exp_mem.hpp"

namespace splicpp
{
	void ir_exp_mem::print(std::ostream& s, const uint tab) const
	{
		s << "MEM(";
		
		print_newline(s, tab+1);
		e->print(s, tab+1);
		
		print_newline(s, tab);
		s << ")";
	}
}

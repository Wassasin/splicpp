#include "ir_exp_eseq.hpp"

#include "ir_stmt.hpp"

namespace splicpp
{
	void ir_exp_eseq::print(std::ostream& s, const uint tab) const
	{
		s << "ESEQ(";
		
		print_newline(s, tab+1);
		e->print(s, tab+1);
		s << ",";
		
		print_newline(s, tab+1);
		this->s->print(s, tab+1);
		
		print_newline(s, tab);
		s << ")";
	}
}

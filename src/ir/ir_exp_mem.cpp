#include "ir_exp_mem.hpp"

namespace splicpp
{
	void ir_exp_mem::print(std::ostream& s, const uint tab) const
	{
		s << "MEM(";
		e->print(s, tab);
		s << ")";
	}
}

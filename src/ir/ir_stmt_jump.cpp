#include "ir_stmt_jump.hpp"

#include "ir_exp.hpp"

namespace splicpp
{
	void ir_stmt_jump::print(std::ostream& s, const uint tab) const
	{
		s << "JUMP(";
		e->print(s, tab);
		s << ")";
	}
}

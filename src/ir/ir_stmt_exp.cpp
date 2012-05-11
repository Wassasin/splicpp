#include "ir_stmt_exp.hpp"

#include "ir_exp.hpp"

namespace splicpp
{
	void ir_stmt_exp::print(std::ostream& s, const uint tab) const
	{
		s << "EXP(";
		e->print(s, tab);
		s << ")";
	}
}

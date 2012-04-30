#include "ir_stmt_label.hpp"

namespace splicpp
{
	void ir_stmt_label::print(std::ostream& s, const uint) const
	{
		s << "LABEL(" << l << ")";
	}
}

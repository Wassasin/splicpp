#include "ir_stmt_label.hpp"

#include "../mappers/generic/ir_stmt_mapper.hpp"

namespace splicpp
{
	void ir_stmt_label::map(ir_stmt_mapper& t) const
	{
		t.map(std::static_pointer_cast<const ir_stmt_label>(shared_from_this()));
	}
	
	void ir_stmt_label::print(std::ostream& s, const uint) const
	{
		s << "LABEL(" << l << ")";
	}
}

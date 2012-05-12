#include "ir_stmt_label.hpp"

#include "../transformers/generic/ir_stmt_transformer.hpp"

namespace splicpp
{
	void ir_stmt_label::transform(ir_stmt_transformer& t) const
	{
		t.transform(std::static_pointer_cast<const ir_stmt_label>(shared_from_this()));
	}
	
	void ir_stmt_label::print(std::ostream& s, const uint) const
	{
		s << "LABEL(" << l << ")";
	}
}

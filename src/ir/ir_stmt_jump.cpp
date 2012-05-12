#include "ir_stmt_jump.hpp"

#include "ir_exp.hpp"

#include "../transformers/generic/ir_stmt_transformer.hpp"

namespace splicpp
{
	void ir_stmt_jump::transform(ir_stmt_transformer& t) const
	{
		t.transform(std::static_pointer_cast<const ir_stmt_jump>(shared_from_this()));
	}
	
	void ir_stmt_jump::print(std::ostream& s, const uint tab) const
	{
		s << "JUMP(";
		e->print(s, tab);
		s << ")";
	}
}

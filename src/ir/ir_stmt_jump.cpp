#include "ir_stmt_jump.hpp"

#include "ir_exp.hpp"

#include "../mappers/generic/ir_stmt_mapper.hpp"

namespace splicpp
{
	void ir_stmt_jump::map(ir_stmt_mapper& t) const
	{
		t.map(std::static_pointer_cast<const ir_stmt_jump>(shared_from_this()));
	}
	
	void ir_stmt_jump::print(std::ostream& s, const uint tab) const
	{
		s << "JUMP(";
		e->print(s, tab);
		s << ")";
	}
}

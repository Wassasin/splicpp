#include "ir_exp_mem.hpp"

#include "../mappers/generic/ir_exp_mapper.hpp"

namespace splicpp
{
	void ir_exp_mem::map(ir_exp_mapper& t) const
	{
		t.map(std::static_pointer_cast<const ir_exp_mem>(shared_from_this()));
	}

	void ir_exp_mem::print(std::ostream& s, const uint tab) const
	{
		s << "MEM(";
		e->print(s, tab);
		s << ")";
	}
}

#include "ir_exp_mem.hpp"

#include "../transformers/generic/ir_exp_transformer.hpp"

namespace splicpp
{
	void ir_exp_mem::transform(ir_exp_transformer& t) const
	{
		t.transform(std::static_pointer_cast<const ir_exp_mem>(shared_from_this()));
	}

	void ir_exp_mem::print(std::ostream& s, const uint tab) const
	{
		s << "MEM(";
		e->print(s, tab);
		s << ")";
	}
}

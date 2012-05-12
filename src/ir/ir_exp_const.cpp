#include "ir_exp_const.hpp"

#include "../transformers/generic/ir_exp_transformer.hpp"

namespace splicpp
{
	void ir_exp_const::transform(ir_exp_transformer& t) const
	{
		t.transform(std::static_pointer_cast<const ir_exp_const>(shared_from_this()));
	}

	void ir_exp_const::print(std::ostream& s, const uint) const
	{
		s << "CONST(" << i << ")";
	}
}

#include "ir_exp_const.hpp"

#include "../mappers/generic/ir_exp_mapper.hpp"

namespace splicpp
{
	void ir_exp_const::map(ir_exp_mapper& t) const
	{
		t.map(std::static_pointer_cast<const ir_exp_const>(shared_from_this()));
	}

	void ir_exp_const::print(std::ostream& s, const uint) const
	{
		s << "CONST(" << i << ")";
	}
}

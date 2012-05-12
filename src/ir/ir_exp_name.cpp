#include "ir_exp_name.hpp"

#include "../mappers/generic/ir_exp_mapper.hpp"

namespace splicpp
{
	void ir_exp_name::map(ir_exp_mapper& t) const
	{
		t.map(std::static_pointer_cast<const ir_exp_name>(shared_from_this()));
	}

	void ir_exp_name::print(std::ostream& s, const uint) const
	{
		s << "NAME(" << l << ")";
	}
}

#include "ir_exp_temp.hpp"

#include "../mappers/generic/ir_exp_mapper.hpp"

namespace splicpp
{
	void ir_exp_temp::map(ir_exp_mapper& t) const
	{
		t.map(std::static_pointer_cast<const ir_exp_temp>(shared_from_this()));
	}

	void ir_exp_temp::print(std::ostream& s, const uint) const
	{
		s << "TEMP(" << t << ")";
	}
}

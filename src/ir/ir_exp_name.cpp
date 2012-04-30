#include "ir_exp_name.hpp"

namespace splicpp
{
	void ir_exp_name::print(std::ostream& s, const uint) const
	{
		s << "NAME(" << l << ")";
	}
}

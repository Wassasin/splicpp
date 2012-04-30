#include "ir_exp_const.hpp"

namespace splicpp
{
	void ir_exp_const::print(std::ostream& s, const uint) const
	{
		s << "CONST(" << i << ")";
	}
}

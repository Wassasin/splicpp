#include "ir_exp_temp.hpp"

namespace splicpp
{
	void ir_exp_temp::print(std::ostream& s, const uint) const
	{
		s << "TEMP(" << t << ")";
	}
}

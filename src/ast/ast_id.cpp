#include "ast_id.hpp"

namespace splicpp
{
	void ast_id::pretty_print(std::ostream& s) const
	{
		s << name;
	}
}

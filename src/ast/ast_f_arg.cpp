#include "ast_f_arg.hpp"

namespace splicpp
{
	void ast_f_arg::pretty_print(std::ostream& s) const
	{
		type->pretty_print(s);
		s << ' ';
		id->pretty_print(s);
	}
}

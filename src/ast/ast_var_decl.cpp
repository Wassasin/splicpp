#include "ast_var_decl.hpp"

namespace splicpp
{
	void ast_var_decl::pretty_print(std::ostream& s, const uint tab) const
	{
		t->pretty_print(s, tab);
		s << ' ';
		id->pretty_print(s, tab);
		s << " = ";
		exp->pretty_print(s, tab);
		s << ";";
	}
}

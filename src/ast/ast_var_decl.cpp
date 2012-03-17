#include "ast_var_decl.hpp"

namespace splicpp
{
	void ast_var_decl::pretty_print(std::ostream& s) const
	{
		t->pretty_print(s);
		s << ' ';
		id->pretty_print(s);
		s << " = ";
		exp->pretty_print(s);
		s << ";";
	}
}

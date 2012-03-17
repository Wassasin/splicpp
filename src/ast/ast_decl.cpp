#include "ast_decl.hpp"

namespace splicpp
{
	ast_decl::ast_decl_type ast_decl_var::type() const
	{
		return t_var_decl;
	}
		
	void ast_decl_var::pretty_print(std::ostream& s, const uint tab) const
	{
		v->pretty_print(s, tab);
	}
	
	ast_decl::ast_decl_type ast_decl_fun::type() const
	{
		return t_fun_decl;
	}
		
	void ast_decl_fun::pretty_print(std::ostream& s, const uint tab) const
	{
		f->pretty_print(s, tab);
	}
}
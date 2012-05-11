#include "ast_decl.hpp"

#include "../typing/varcontext.hpp"
#include "../typing/symboltable.hpp"

#include "ast_var_decl.hpp"

namespace splicpp
{
	/* ast_decl_var */

	sid ast_decl_var::fetch_id() const
	{
		return v->fetch_id();
	}

	void ast_decl_var::register_globals(symboltable& s, varcontext& c)
	{
		c.assign(v->fetch_name(), s.reg_var(v));
	}
	
	void ast_decl_var::register_locals(symboltable& s, varcontext& c)
	{
		varcontext ctype; //Fresh context
		v->register_types(s, ctype);
		
		v->assign_ids(c);
	}
	
	ast_decl::ast_decl_type ast_decl_var::type() const
	{
		return t_var_decl;
	}
		
	void ast_decl_var::pretty_print(std::ostream& s, const uint tab) const
	{
		v->pretty_print(s, tab);
	}
	
	/* ast_decl_fun */
	
	sid ast_decl_fun::fetch_id() const
	{
		return f->fetch_id();
	}
	
	void ast_decl_fun::register_globals(symboltable& s, varcontext& c)
	{
		c.assign(f->fetch_name(), s.reg_fun(f));
	}
	
	void ast_decl_fun::register_locals(symboltable& s, varcontext& c)
	{
		f->register_locals(s, c);
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

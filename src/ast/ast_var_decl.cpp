#include "ast_var_decl.hpp"

#include "../typing/types/sl_type.hpp"

#include "ast_id.hpp"
#include "ast_exp.hpp"
#include "ast_type.hpp"

namespace splicpp
{
	void ast_var_decl::assign(sid i)
	{
		id->assign(i);
	}
	
	std::string ast_var_decl::fetch_name() const
	{
		return id->fetch_name();
	}
	
	void ast_var_decl::assign_ids(const varcontext& c)
	{
		exp->assign_ids(c);
	}
	
	void ast_var_decl::register_types(symboltable& s, varcontext& c)
	{
		ast_type::register_type(t, s, c);
	}
	
	std::shared_ptr<sl_type> ast_var_decl::fetch_assigned_type(const typecontext& c) const
	{
		return t->fetch_type(c)->qualify(c);
	}
	
	substitution ast_var_decl::infer_type(const typecontext& c) const
	{
		return exp->infer_type(c, t->fetch_type(c));
	}

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

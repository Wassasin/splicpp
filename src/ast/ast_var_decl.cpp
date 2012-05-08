#include "ast_var_decl.hpp"

#include "../typing/typecontext.hpp"
#include "../typing/ltypecontext.hpp"

#include "../typing/types/sl_polytype.hpp"
#include "../typing/types/sl_type.hpp"
#include "../typing/types/sl_type_unbound.hpp"

#include "ast_id.hpp"
#include "ast_exp.hpp"
#include "ast_type.hpp"

#include "../ir/ircontext.hpp"
#include "../ir/ir_stmt_move.hpp"

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
	
	sid ast_var_decl::fetch_id() const
	{
		return id->fetch_id();
	}
	
	void ast_var_decl::assign_ids(const varcontext& c)
	{
		exp->assign_ids(c);
	}
	
	void ast_var_decl::register_types(symboltable& s, varcontext& c)
	{
		ast_type::register_type(t, s, c);
	}
	
	s_ptr<const sl_type> ast_var_decl::fetch_assigned_type(const typecontext& c) const
	{
		return t->fetch_type(c);
	}
	
	substitution ast_var_decl::declare_type(ltypecontext& c) const
	{
		const s_ptr<const sl_type_unbound> a = c.create_fresh(sl);
		const substitution s = exp->infer_type(c, a);
		const s_ptr<const sl_polytype> t = sl_polytype::qualify(c.apply(s), a->apply(s));
		
		c.register_type(id->fetch_id(), t);
		return s;
	}
	
	s_ptr<const ir_stmt> ast_var_decl::translate(const ircontext& c) const
	{
		return ir_stmt_move::create(
			c.fetch_memloc(id->fetch_id()),
			exp->translate(c)
		);
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

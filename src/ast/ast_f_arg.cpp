#include "ast_f_arg.hpp"

#include "ast_id.hpp"
#include "ast_type.hpp"

#include "../typing/typecontext.hpp"
#include "../typing/types/sl_polytype.hpp"
#include "../typing/types/sl_type.hpp"
#include "../typing/types/sl_type_unbound.hpp"

namespace splicpp
{
	void ast_f_arg::assign(sid i)
	{
		id->assign(i);
	}
	
	std::string ast_f_arg::fetch_name() const
	{
		return id->fetch_name();
	}
	
	sid ast_f_arg::fetch_id() const
	{
		return id->fetch_id();
	}
	
	void ast_f_arg::register_types(symboltable& s, varcontext& c)
	{
		ast_type::register_type(t, s, c);
	}
	
	cs_ptr<sl_type> ast_f_arg::fetch_assigned_type(const typecontext& c) const
	{
		return t->fetch_type(c);
	}
	
	cs_ptr<sl_type_unbound> ast_f_arg::declare_type(typecontext& c) const
	{
		const auto t = c.create_fresh(sl);
		c.register_type(id->fetch_id(), sl_polytype::not_qualify(t)); //t is fresh, thus would always be qualified
		return t;
	}

	void ast_f_arg::pretty_print(std::ostream& s, const uint tab) const
	{
		t->pretty_print(s, tab);
		s << ' ';
		id->pretty_print(s, tab);
	}
}

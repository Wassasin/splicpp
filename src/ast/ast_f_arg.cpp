#include "ast_f_arg.hpp"

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
	
	void ast_f_arg::register_types(symboltable& s, varcontext& c)
	{
		ast_type::register_type(t, s, c);
	}
	
	std::shared_ptr<sl_type> ast_f_arg::fetch_assigned_type(const typecontext& c) const
	{
		return t->fetch_type(c);
	}

	void ast_f_arg::pretty_print(std::ostream& s, const uint tab) const
	{
		t->pretty_print(s, tab);
		s << ' ';
		id->pretty_print(s, tab);
	}
}

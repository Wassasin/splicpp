#include "ast_id.hpp"

#include <sstream>

#include "../common/errors.hpp"
#include "../typing/varcontext.hpp"

namespace splicpp
{
	void ast_id::pretty_print(std::ostream& s, const uint) const
	{
		s << name;
	}
	
	void ast_id::assign(sid i)
	{
		if(id)
			throw parse_error("ast_id already has symbolidentifier assigned");
		
		id = i;
	}
	
	void ast_id::assign_ids(const varcontext& c)
	{
		if(!c.exists(name))
		{
			std::stringstream s;
			s << "Use of undefined variable \"" << name << "\"";
			throw parse_error(s.str(), sl);
		}
		assign(c[name]);
	}
	
	sid ast_id::fetch_id() const
	{
		return id.get();
	}
	
	std::string ast_id::fetch_name() const
	{
		return name;
	}
}

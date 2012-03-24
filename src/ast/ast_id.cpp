#include "ast_id.hpp"

#include "../common/errors.hpp"

#include <sstream>

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
			throw parse_error(s.str());
		}
		assign(c[name]);
	}
	
	std::string ast_id::fetch_name() const
	{
		return name;
	}
}

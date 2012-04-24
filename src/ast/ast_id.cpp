#include "ast_id.hpp"

#include <sstream>

#include "../common/errors.hpp"
#include "../typing/varcontext.hpp"
#include "../typing/typecontext.hpp"

#include "../typing/types/sl_polytype.hpp"
#include "../typing/types/sl_type.hpp"

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
	
	substitution ast_id::infer_type(const typecontext& c, const cs_ptr<sl_type> t) const
	{
		const cs_ptr<sl_type> xt = c[fetch_id()]->unbind(c);
		const substitution s = xt->unify(t);
		
		std::cout << std::endl;
		std::cout << std::endl << "ast_id::infer_type name: " << name;
		std::cout << std::endl << "ast_id::infer_type xt (pre-unbind): ";
		c[fetch_id()]->print(std::cout);
		std::cout << std::endl << "ast_id::infer_type xt: ";
		xt->print(std::cout);
		std::cout << std::endl << "ast_id::infer_type t: ";
		t->print(std::cout);
		std::cout << std::endl << "ast_id::infer_type s: ";
		s.print(std::cout);
		std::cout << std::endl;
		
		return s;
	}
}

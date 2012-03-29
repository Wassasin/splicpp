#ifndef AST_ID_H
#define AST_ID_H

#include <boost/optional.hpp>

#include "../common/typedefs.hpp"

#include "ast.hpp"

namespace splicpp
{
	class symboltable;
	class varcontext;

	class ast_id : public ast
	{
		std::string name;
		boost::optional<sid> id;

	public:	
		ast_id(std::string name, const sloc sl)
		: ast(sl)
		, name(name)
		, id()
		{}
		
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		void assign(sid i);
		void assign_ids(const varcontext& c);
		sid fetch_id() const;
		
		std::string fetch_name() const;
	};
}

#endif

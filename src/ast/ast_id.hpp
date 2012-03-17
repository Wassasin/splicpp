#ifndef AST_ID_H
#define AST_ID_H

#include "ast.hpp"

namespace splicpp
{
	class ast_id : public ast
	{
		std::string name;
		//uint id; //TODO Unique identifier, name is not unique per se

	public:	
		ast_id(std::string name)
		: name(name)
		{}
		
		virtual void pretty_print(std::ostream& s) const;
	};
}

#endif

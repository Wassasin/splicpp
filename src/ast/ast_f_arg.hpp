#ifndef AST_F_ARG_H
#define AST_F_ARG_H

#include "ast.hpp"

#include "ast_type.hpp"
#include "ast_id.hpp"

namespace splicpp
{
	class ast_f_arg : public ast
	{
		std::shared_ptr<ast_type> type;
		std::shared_ptr<ast_id> id;

	public:	
		ast_f_arg(__decltype(type) type, __decltype(id) id)
		: type(type)
		, id(id)
		{}
		
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

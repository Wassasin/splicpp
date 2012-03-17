#ifndef AST_FUN_CALL_H
#define AST_FUN_CALL_H

#include <memory>
#include <vector>

#include "ast.hpp"

#include "ast_id.hpp"
#include "ast_exp.hpp"

namespace splicpp
{
	class ast_exp;

	class ast_fun_call : public ast
	{
		std::shared_ptr<ast_id> id;
		std::vector<std::shared_ptr<ast_exp>> args;
		
	public:
		ast_fun_call(__decltype(id) id)
		: id(id)
		, args()
		{}
		
		void add_arg(std::shared_ptr<ast_exp> exp);
		virtual void pretty_print(std::ostream& s) const;
	};
}

#endif

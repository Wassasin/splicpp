#ifndef AST_FUN_CALL_H
#define AST_FUN_CALL_H

#include <memory>
#include <vector>

#include "ast.hpp"

#include "ast_id.hpp"
#include "ast_exp.hpp"

#include "../typing/symboltable.hpp"
#include "../typing/varcontext.hpp"

namespace splicpp
{
	class symboltable;
	class varcontext;
	class ast_id;
	class ast_exp;

	class ast_fun_call : public ast
	{
		std::shared_ptr<ast_id> id;
		std::vector<std::shared_ptr<ast_exp>> args;
		
	public:
		ast_fun_call(__decltype(id) id, const sloc sl)
		: ast(sl)
		, id(id)
		, args()
		{}
		
		void add_arg(std::shared_ptr<ast_exp> exp);
		
		void assign_ids(const varcontext& c);
		
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

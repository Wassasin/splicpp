#ifndef AST_FUN_CALL_H
#define AST_FUN_CALL_H

#include <memory>
#include <vector>

#include "ast.hpp"

#include "../typing/substitution.hpp"

namespace splicpp
{
	class symboltable;
	class varcontext;
	class typecontext;
	class ast_id;
	class ast_exp;

	class ast_fun_call : public ast
	{
		s_ptr<ast_id> id;
		std::vector<s_ptr<ast_exp>> args;
		
	public:
		ast_fun_call(__decltype(id) id, const sloc sl)
		: ast(sl)
		, id(id)
		, args()
		{}
		
		void add_arg(s_ptr<ast_exp> exp);
		
		void assign_ids(const varcontext& c);
		substitution infer_type(const typecontext& c, const s_ptr<sl_type> t) const;
		
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

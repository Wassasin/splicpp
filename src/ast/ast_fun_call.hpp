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
	class ir_stmt;
	class ircontext;

	class ast_fun_call : public ast
	{
	public:
		const s_ptr<ast_id> id;
		const std::vector<s_ptr<ast_exp>> args;
		
		ast_fun_call(decltype(id) id, decltype(args) args, const sloc sl)
		: ast(sl)
		, id(id)
		, args(args)
		{}
		
		void assign_ids(const varcontext& c);
		substitution infer_type(const typecontext& c, const s_ptr<const sl_type> t) const;
		
		s_ptr<const ir_stmt> translate(const ircontext& c) const;
		s_ptr<const ir_stmt> translate(const ir_temp return_reg, const ircontext& c) const;
		
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

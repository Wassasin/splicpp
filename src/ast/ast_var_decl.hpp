#ifndef AST_VAR_DECL_H
#define AST_VAR_DECL_H

#include "ast.hpp"

#include "ast_type.hpp"
#include "ast_id.hpp"
#include "ast_exp.hpp"

namespace splicpp
{
	class ast_var_decl : public ast
	{
		std::shared_ptr<ast_type> t;
		std::shared_ptr<ast_id> id;
		std::shared_ptr<ast_exp> exp;
	
	public:
		ast_var_decl(__decltype(t) t, __decltype(id) id, __decltype(exp) exp)
		: t(t)
		, id(id)
		, exp(exp)
		{}
		
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

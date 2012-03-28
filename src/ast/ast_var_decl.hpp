#ifndef AST_VAR_DECL_H
#define AST_VAR_DECL_H

#include <memory>

#include "ast.hpp"

#include "ast_type.hpp"
#include "ast_id.hpp"
#include "ast_exp.hpp"

#include "../typing/symboltable.hpp"
#include "../typing/varcontext.hpp"

namespace splicpp
{
	class symboltable;
	class varcontext;
	class ast_type;
	class ast_exp;
	class sl_type;
	class typecontext;

	class ast_var_decl : public ast
	{
		std::shared_ptr<ast_type> t;
		std::shared_ptr<ast_id> id;
		std::shared_ptr<ast_exp> exp;
	
	public:
		ast_var_decl(__decltype(t) t, __decltype(id) id, __decltype(exp) exp, const sloc sl)
		: ast(sl)
		, t(t)
		, id(id)
		, exp(exp)
		{}
		
		void assign(sid i);
		std::string fetch_name() const;
		
		void assign_ids(const varcontext& c);
		void register_types(symboltable& s, varcontext& c);
		
		std::shared_ptr<sl_type> fetch_assigned_type(const typecontext& c) const;
		
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

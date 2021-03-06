#ifndef AST_VAR_DECL_H
#define AST_VAR_DECL_H

#include <memory>

#include "ast.hpp"

#include "../common/typedefs.hpp"
#include "../typing/substitution.hpp"

namespace splicpp
{
	class ast_type;
	class ast_exp;
	class ast_id;
	class symboltable;
	class varcontext;
	class typecontext;
	class ltypecontext;
	class sl_type;
	class ir_stmt;
	class ircontext;

	class ast_var_decl : public ast
	{
	public:
		const s_ptr<ast_type> t;
		const s_ptr<ast_id> id;
		const s_ptr<ast_exp> exp;
	
		ast_var_decl(__decltype(t) t, __decltype(id) id, __decltype(exp) exp, const sloc sl)
		: ast(sl)
		, t(t)
		, id(id)
		, exp(exp)
		{}
		
		void assign(sid i);
		std::string fetch_name() const;
		sid fetch_id() const;
		
		void assign_ids(const varcontext& c);
		void register_types(symboltable& s, varcontext& c);
		
		s_ptr<const sl_type> fetch_assigned_type(const typecontext& c) const;
		substitution declare_type(ltypecontext& c) const;
		
		s_ptr<const ir_stmt> translate(const ircontext& c) const;
		
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

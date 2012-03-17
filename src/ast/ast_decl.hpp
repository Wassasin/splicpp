#ifndef AST_DECL_H
#define AST_DECL_H

#include "ast.hpp"

#include "ast_fun_decl.hpp"
#include "ast_var_decl.hpp"

namespace splicpp
{
	class ast_decl : public ast
	{
	public:
		enum ast_decl_type
		{
			t_var_decl,
			t_fun_decl
		};
		
		virtual ast_decl_type type() const = 0;
		virtual void pretty_print(std::ostream& s) const = 0;
	};
	
	class ast_decl_var : public ast_decl
	{
		std::shared_ptr<ast_var_decl> v;
	
	public:
		ast_decl_var(__decltype(v) v)
		: v(v)
		{}
	
		virtual ast_decl_type type() const;
		virtual void pretty_print(std::ostream& s) const;
	};
	
	class ast_decl_fun : public ast_decl
	{
		std::shared_ptr<ast_fun_decl> f;
		
	public:
		ast_decl_fun(__decltype(f) f)
		: f(f)
		{}
		
		virtual ast_decl_type type() const;
		virtual void pretty_print(std::ostream& s) const;
	};
}

#endif

#ifndef AST_DECL_H
#define AST_DECL_H

#include "../common/typedefs.hpp"

#include "ast.hpp"

namespace splicpp
{
	class symboltable;
	class varcontext;
	class ast_fun_decl;
	class ast_var_decl;
	
	class ast_decl : public ast
	{
	public:
		enum ast_decl_type
		{
			t_var_decl,
			t_fun_decl
		};
		
		ast_decl(const sloc sl)
		: ast(sl)
		{}
		
		virtual void register_globals(symboltable&, varcontext& c) = 0;
		virtual void register_locals(symboltable&, varcontext& c) = 0;
		
		virtual ast_decl_type type() const = 0;
		virtual void pretty_print(std::ostream& s, const uint tab) const = 0;
	};
	
	class ast_decl_var : public ast_decl
	{
		std::shared_ptr<ast_var_decl> v;
	
	public:
		ast_decl_var(__decltype(v) v, const sloc sl)
		: ast_decl(sl)
		, v(v)
		{}
	
		virtual void register_globals(symboltable&, varcontext& c);
		virtual void register_locals(symboltable&, varcontext& c);
		
		virtual ast_decl_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
	
	class ast_decl_fun : public ast_decl
	{
		std::shared_ptr<ast_fun_decl> f;
		
	public:
		ast_decl_fun(__decltype(f) f, const sloc sl)
		: ast_decl(sl)
		, f(f)
		{}
		
		virtual void register_globals(symboltable&, varcontext& c);
		virtual void register_locals(symboltable&, varcontext& c);
		
		virtual ast_decl_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

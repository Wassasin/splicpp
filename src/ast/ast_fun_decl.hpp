#ifndef AST_FUN_DECL_H
#define AST_FUN_DECL_H

#include <boost/optional.hpp>

#include "ast.hpp"

#include "ast_type.hpp"
#include "ast_id.hpp"
#include "ast_f_arg.hpp"
#include "ast_var_decl.hpp"
#include "ast_stmt.hpp"

#include "../typing/symboltable.hpp"
#include "../typing/varcontext.hpp"

namespace splicpp
{
	class ast_f_arg;
	class ast_var_decl;
	class ast_stmt;
	class symboltable;
	class varcontext;

	class ast_fun_decl : public ast
	{
		boost::optional<std::shared_ptr<ast_type>> t;
		std::shared_ptr<ast_id> id;
		
		std::vector<std::shared_ptr<ast_f_arg>> args;
		std::vector<std::shared_ptr<ast_var_decl>> decls;
		std::vector<std::shared_ptr<ast_stmt>> stmts;
	
	public:
		ast_fun_decl(__decltype(t) t, __decltype(id) id, const sloc sl)
		: ast(sl)
		, t(t)
		, id(id)
		, args()
		, decls()
		, stmts()
		{}
		
		ast_fun_decl(__decltype(id) id, const sloc sl)
		: ast(sl)
		, t()
		, id(id)
		, args()
		, decls()
		, stmts()
		{}
		
		void add_arg(std::shared_ptr<ast_f_arg> arg);
		void add_decl(std::shared_ptr<ast_var_decl> decl);
		void add_stmt(std::shared_ptr<ast_stmt> stmt);
		
		void assign(sid i);
		std::string fetch_name() const;
		
		void register_locals(symboltable& s, varcontext& c);
		
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

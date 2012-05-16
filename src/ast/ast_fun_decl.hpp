#ifndef AST_FUN_DECL_H
#define AST_FUN_DECL_H

#include <boost/optional.hpp>
#include <memory>
#include <vector>

#include "ast.hpp"

#include "../common/typedefs.hpp"
#include "../typing/substitution.hpp"

namespace splicpp
{
	class ast_f_arg;
	class ast_var_decl;
	class ast_stmt;
	class ast_type;
	class ast_id;
	class symboltable;
	class varcontext;
	class sl_type;
	class typecontext;
	class ltypecontext;
	class ircontext;
	class ir_stmt;

	class ast_fun_decl : public ast
	{
	public:
		const s_ptr<ast_type> t;
		const s_ptr<ast_id> id;
		
		const std::vector<s_ptr<ast_f_arg>> args;
		const std::vector<s_ptr<ast_var_decl>> decls;
		const std::vector<s_ptr<ast_stmt>> stmts;
	
		ast_fun_decl(__decltype(t) t, __decltype(id) id, __decltype(args) args, __decltype(decls) decls, __decltype(stmts) stmts, const sloc sl)
		: ast(sl)
		, t(t)
		, id(id)
		, args(args)
		, decls(decls)
		, stmts(stmts)
		{}
		
		void assign(sid i);
		std::string fetch_name() const;
		sid fetch_id() const;
		
		void register_locals(symboltable& s, varcontext& c);
		
		s_ptr<const sl_type> fetch_assigned_type(const typecontext& c) const;
		substitution declare_type(ltypecontext& c) const;
		
		s_ptr<const ir_stmt> translate(const ir_label l_function, const ircontext& c) const;
		
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

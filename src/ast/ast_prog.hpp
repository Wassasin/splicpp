#ifndef AST_PROG_H
#define AST_PROG_H

#include <vector>

#include "../common/typedefs.hpp"

#include "ast.hpp"

namespace splicpp
{
	class ast_decl;
	class ast_decl_fun;
	class ast_construct;
	class symboltable;
	class varcontext;
	class ircontext;
	class ircontext;
	class ir_stmt;
	
	class ast_prog : public ast
	{
		static std::vector<s_ptr<ast_construct>> init_constrs();
		
		s_ptr<ast_decl_fun> fetch_main() const;
	public:
		const std::vector<s_ptr<ast_decl>> decls;
		const std::vector<s_ptr<ast_construct>> constrs;
	
		ast_prog(const std::vector<s_ptr<ast_decl>> decls, const sloc sl)
		: ast(sl)
		, decls(decls)
		, constrs(init_constrs())
		{}
		
		void register_ids(symboltable& s);
		void register_globals(symboltable&, varcontext& c);
		void register_locals(symboltable&, varcontext& c);
		
		s_ptr<const ir_stmt> translate(ircontext& c, const symboltable& st) const;
		
		void pretty_print(std::ostream& s) const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

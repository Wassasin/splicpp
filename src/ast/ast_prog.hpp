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
		std::vector<s_ptr<ast_decl>> decls;
		std::vector<s_ptr<ast_construct>> constrs;
	
		static std::vector<s_ptr<ast_construct>> init_constrs();
		
		s_ptr<ast_decl_fun> fetch_main() const;
	public:
		ast_prog(const sloc sl)
		: ast(sl)
		, decls()
		, constrs(init_constrs())
		{}
		
		void add_decl(s_ptr<ast_decl> decl);
		
		void register_ids(symboltable& s);
		void register_globals(symboltable&, varcontext& c);
		void register_locals(symboltable&, varcontext& c);
		
		s_ptr<const ir_stmt> translate(ircontext& c) const;
		
		void pretty_print(std::ostream& s) const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

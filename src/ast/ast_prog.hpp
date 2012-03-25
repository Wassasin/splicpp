#ifndef AST_PROG_H
#define AST_PROG_H

#include "ast.hpp"
#include "ast_decl.hpp"

namespace splicpp
{
	class ast_prog : public ast
	{
		std::vector<std::shared_ptr<ast_decl>> decls;
	
	public:
		ast_prog(const sloc sl)
		: ast(sl)
		, decls()
		{}
		
		void add_decl(std::shared_ptr<ast_decl> decl);
		
		void register_ids(symboltable& s);
		void register_globals(symboltable&, varcontext& c);
		void register_locals(symboltable&, varcontext& c);
		
		void pretty_print(std::ostream& s) const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif
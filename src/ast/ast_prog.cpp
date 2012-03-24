#include "ast_prog.hpp"

namespace splicpp
{
	void ast_prog::add_decl(std::shared_ptr<ast_decl> decl)
	{
		decls.push_back(decl);
	}
	
	void ast_prog::register_ids(symboltable& s)
	{
		varcontext c;
		
		register_globals(s, c);
		register_locals(s, c);
	}
	
	void ast_prog::register_globals(symboltable& s, varcontext& c)
	{
		for(auto decl : decls)
			decl->register_globals(s, c);
	}
	
	void ast_prog::register_locals(symboltable& s, varcontext& c)
	{
		for(auto decl : decls)
			decl->register_locals(s, c);
	}
	
	void ast_prog::pretty_print(std::ostream& s) const
	{
		pretty_print(s, 0);
	}
	
	void ast_prog::pretty_print(std::ostream& s, const uint tab) const
	{
		for(const auto decl : decls)
		{
			decl->pretty_print(s, tab);
			s << std::endl;
		}
	}
	
	
}

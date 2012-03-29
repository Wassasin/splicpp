#include "ast_prog.hpp"

#include "../typing/varcontext.hpp"
#include "../typing/symboltable.hpp"

#include "ast_decl.hpp"

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
		//Define language constructs
		std::shared_ptr<ast_construct> cons;
		
		cons = decltype(cons)(new ast_construct_print());
		c.assign(cons->fetch_name(), s.reg_cons(cons));
		
		cons = decltype(cons)(new ast_construct_is_empty());
		c.assign(cons->fetch_name(), s.reg_cons(cons));
		
		cons = decltype(cons)(new ast_construct_head());
		c.assign(cons->fetch_name(), s.reg_cons(cons));
		
		cons = decltype(cons)(new ast_construct_tail());
		c.assign(cons->fetch_name(), s.reg_cons(cons));
		
		cons = decltype(cons)(new ast_construct_fst());
		c.assign(cons->fetch_name(), s.reg_cons(cons));
		
		cons = decltype(cons)(new ast_construct_snd());
		c.assign(cons->fetch_name(), s.reg_cons(cons));
	
		//Register globals
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

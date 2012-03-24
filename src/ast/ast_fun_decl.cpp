#include "ast_fun_decl.hpp"

namespace splicpp
{
	void ast_fun_decl::add_arg(std::shared_ptr<ast_f_arg> arg)
	{
		args.push_back(arg);
	}
	
	void ast_fun_decl::add_decl(std::shared_ptr<ast_var_decl> decl)
	{
		decls.push_back(decl);
	}
	
	void ast_fun_decl::add_stmt(std::shared_ptr<ast_stmt> stmt)
	{
		stmts.push_back(stmt);
	}
	
	void ast_fun_decl::assign(sid i)
	{
		id->assign(i);
	}
	
	std::string ast_fun_decl::fetch_name() const
	{
		return id->fetch_name();
	}
	
	void ast_fun_decl::register_locals(symboltable& s, varcontext& c)
	{
		varcontext cvar = c;
		varcontext ctype; //Fresh context
		
		if(t)
			ast_type::register_type(t.get(), s, ctype);
		
		for(auto arg : args)
		{
			cvar.assign(arg->fetch_name(), s.reg_arg(arg));
			arg->register_types(s, ctype);
		}
		
		for(auto decl : decls)
		{
			cvar.assign(decl->fetch_name(), s.reg_lvar(decl));
			decl->register_types(s, ctype);
		}
		
		for(auto stmt : stmts)
			stmt->assign_ids(cvar);
		/*
		std::cout << id->fetch_name() << ": " << std::endl;
		cvar.print(std::cout);
		
		std::cout << id->fetch_name() << " (types): " << std::endl;
		ctype.print(std::cout);
		*/
	}
	
	void ast_fun_decl::pretty_print(std::ostream& s, const uint tab) const
	{
		if(t)
			t.get()->pretty_print(s, tab);
		else
			s << "Void";
		
		s << ' ';
		id->pretty_print(s, tab);
		s << '(';
		
		bool first = true;
		for(const auto arg : args)
		{
			if(first)
				first = false;
			else
				s << ", ";
			
			arg->pretty_print(s, tab);
		}
		
		s << ')';
		ast::print_newline(s, tab);
		s << '{';
		
		for(const auto decl : decls)
		{
			ast::print_newline(s, tab+1);
			decl->pretty_print(s, tab+1);
		}
		
		for(const auto stmt : stmts)
		{
			ast::print_newline(s, tab+1);
			stmt->pretty_print(s, tab+1);
		}
		
		ast::print_newline(s, tab);
		s << '}';
	}
}

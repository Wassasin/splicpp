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

	void ast_fun_decl::pretty_print(std::ostream& s) const
	{
		if(t)
			t.get()->pretty_print(s);
		else
			s << "Void";
		
		s << ' ';
		id->pretty_print(s);
		s << '(';
		
		bool first = true;
		for(const auto arg : args)
		{
			if(first)
				first = false;
			else
				s << ", ";
			
			arg->pretty_print(s);
		}
		
		s << ')' << std::endl << '{';
		
		for(const auto decl : decls)
			decl->pretty_print(s);
		
		for(const auto stmt : stmts)
			stmt->pretty_print(s);
		
		s << '}';
	}
}

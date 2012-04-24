#include "ast_fun_decl.hpp"

#include "../typing/typecontext.hpp"
#include "../typing/ltypecontext.hpp"

#include "../typing/varcontext.hpp"
#include "../typing/symboltable.hpp"

#include "../typing/types/sl_polytype.hpp"
#include "../typing/types/sl_type.hpp"
#include "../typing/types/sl_type_function.hpp"
#include "../typing/types/sl_type_unbound.hpp"

#include "ast_id.hpp"
#include "ast_f_arg.hpp"
#include "ast_stmt.hpp"

namespace splicpp
{
	void ast_fun_decl::add_arg(s_ptr<ast_f_arg> arg)
	{
		args.push_back(arg);
	}
	
	void ast_fun_decl::add_decl(s_ptr<ast_var_decl> decl)
	{
		decls.push_back(decl);
	}
	
	void ast_fun_decl::add_stmt(s_ptr<ast_stmt> stmt)
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
		
		ast_type::register_type(t, s, ctype);
		
		for(auto arg : args)
		{
			cvar.assign(arg->fetch_name(), s.reg_arg(arg));
			arg->register_types(s, ctype);
		}
		
		for(auto decl : decls)
		{
			decl->assign_ids(cvar);
			cvar.assign(decl->fetch_name(), s.reg_lvar(decl));
			decl->register_types(s, ctype);
		}
		
		for(auto stmt : stmts)
			stmt->assign_ids(cvar);
	}
	
	cs_ptr<sl_type> ast_fun_decl::fetch_assigned_type(const typecontext& c) const
	{
		std::vector<cs_ptr<sl_type>> t_args;
		for(const auto arg : args)
			t_args.push_back(arg->fetch_assigned_type(c));
		
		return cs_ptr<sl_type>(new sl_type_function(t_args, this->t->fetch_type(c)));
	}
	
	substitution ast_fun_decl::declare_type(ltypecontext& c) const
	{
		ltypecontext clocal = c;
	
		cs_ptr<sl_type_unbound> r = c.create_fresh();
		std::vector<cs_ptr<sl_type>> t_args;
		for(size_t i = 0; i < args.size(); i++)
			t_args.push_back(args[i]->declare_type(clocal));
		
		cs_ptr<sl_type_function> ft(new sl_type_function(t_args, r));
		
		substitution s;
		for(const auto decl : decls)
		{
			clocal = clocal.apply(s);
			s = decl->declare_type(clocal).composite(s);
		}
		
		for(const auto stmt : stmts)
		{
			std::cout << std::endl << "BLAAT";
			clocal.apply(s).print(std::cout << std::endl);
			s = stmt->infer_type(clocal.apply(s), r->apply(s)).composite(s);
		}
		
		std::cout << std::endl;
		std::cout << std::endl << "ast_fun_decl::declare_type register_type: ";
		sl_polytype::qualify(c.apply(s), ft->apply(s))->print(std::cout);
		
		c.register_type(id->fetch_id(), sl_polytype::qualify(c.apply(s), ft->apply(s)));
		
		std::cout << std::endl << "ast_fun_decl::declare_type c: ";
		c.print(std::cout);
		
		return s;
	}
	
	void ast_fun_decl::pretty_print(std::ostream& s, const uint tab) const
	{
		t->pretty_print(s, tab);
		
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

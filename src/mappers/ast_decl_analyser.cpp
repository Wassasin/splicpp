#include "ast_decl_analyser.hpp"

#include <stdexcept>

#include "../ast/ast_decl.hpp"
#include "../ast/ast_fun_decl.hpp"
#include "../ast/ast_var_decl.hpp"

#include "../ast/ast_exp.hpp"
#include "../ast/ast_stmt.hpp"

#include "../ast/ast_id.hpp"
#include "../ast/ast_fun_call.hpp"

#include "../typing/symboltable.hpp"

namespace splicpp
{
	dgraph<sid> ast_decl_analyser::analyse(const std::vector<s_ptr<ast_decl>>& decls, const symboltable& t)
	{
		ast_decl_analyser a;
		
		//Initialize graph
		for(const sid i : t.select_all(symboltable::symbolref::t_fun))
			a.g.add_vertex(i);
		
		for(const sid i : t.select_all(symboltable::symbolref::t_construct))
			a.g.add_vertex(i);
		
		for(const sid i : t.select_all(symboltable::symbolref::t_var))
			a.g.add_vertex(i);
		
		for(const sid i : t.select_all(symboltable::symbolref::t_arg))
			a.g.add_vertex(i);
		
		for(const sid i : t.select_all(symboltable::symbolref::t_local_var))
			a.g.add_vertex(i);
		
		//Analyse AST
		for(const auto decl : decls)
			if(decl->type() == ast_decl::t_var_decl)
				a.analyse(std::dynamic_pointer_cast<ast_decl_var>(decl));
			else
				a.analyse(std::dynamic_pointer_cast<ast_decl_fun>(decl));

		//Remove non-decls from graph
		for(const sid i : t.select_all(symboltable::symbolref::t_construct))
			a.g.remove_vertex(i);
			
		for(const sid i : t.select_all(symboltable::symbolref::t_arg))
			a.g.remove_vertex(i);
		
		for(const sid i : t.select_all(symboltable::symbolref::t_local_var))
			a.g.remove_vertex(i);
		
		return a.g;
	}
	
	void ast_decl_analyser::analyse(const s_ptr<const ast_decl_fun> x)
	{
		const s_ptr<ast_fun_decl> f = x->f;
		
		i = f->id->fetch_id();
		
		for(const auto decl : f->decls)
			analyse(decl->exp);
		
		for(const auto stmt : f->stmts)
			analyse(stmt);
	}
	
	void ast_decl_analyser::analyse(const s_ptr<const ast_decl_var> x)
	{
		const s_ptr<ast_var_decl> v = x->v;
		
		i = v->id->fetch_id();
		analyse(v->exp);
	}
		
	void ast_decl_analyser::analyse(const s_ptr<const ast_exp> x)
	{
		x->map(*this);
	}
	
	void ast_decl_analyser::analyse(const s_ptr<const ast_stmt> x)
	{
		x->map(*this);
	}
	
	//Inherited from ast_exp_mapper
	void ast_decl_analyser::map(const s_ptr<const ast_exp_id> x)
	{
		g.add_edge(i, x->id->fetch_id());
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_exp_op2> x)
	{
		analyse(x->e_left);
		analyse(x->e_right);
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_exp_negation> x)
	{
		analyse(x->exp);
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_exp_int>)
	{
		//Do nothing
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_exp_bool>)
	{
		//Do nothing
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_exp_exp> x)
	{
		analyse(x->exp);
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_exp_fun_call> x)
	{
		g.add_edge(i, x->c->id->fetch_id());
	
		for(const auto arg : x->c->args)
			analyse(arg);
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_exp_nil>)
	{
		//Dp nothing
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_exp_tuple> x)
	{
		analyse(x->e_left);
		analyse(x->e_right);
	}
	
	//Inherited from ast_stmt_mapper
	void ast_decl_analyser::map(const s_ptr<const ast_stmt_stmts> x)
	{
		for(const auto stmt : x->stmts)
			analyse(stmt);
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_stmt_if> x)
	{
		analyse(x->exp);
		analyse(x->stmt_true);
		
		if(x->stmt_false)
			analyse(x->stmt_false.get());
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_stmt_while> x)
	{
		analyse(x->exp);
		analyse(x->stmt);
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_stmt_assignment> x)
	{
		analyse(x->exp);
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_stmt_fun_call> x)
	{
		g.add_edge(i, x->f->id->fetch_id());
	
		for(const auto arg : x->f->args)
			analyse(arg);
	}
	
	void ast_decl_analyser::map(const s_ptr<const ast_stmt_return> x)
	{
		if(x->exp)
			analyse(x->exp.get());
	}
}

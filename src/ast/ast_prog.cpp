#include "ast_prog.hpp"

#include "../typing/varcontext.hpp"
#include "../typing/symboltable.hpp"

#include "ast_decl.hpp"

#include "../ir/ircontext.hpp"
#include "../ir/ir_exp_name.hpp"
#include "../ir/ir_exp_temp.hpp"
#include "../ir/ir_stmt_label.hpp"
#include "../ir/ir_stmt_move.hpp"

namespace splicpp
{
	std::vector<s_ptr<ast_construct>> ast_prog::init_constrs()
	{
		std::vector<s_ptr<ast_construct>> result;
		
		result.push_back(s_ptr<ast_construct>(new ast_construct_print()));
		result.push_back(s_ptr<ast_construct>(new ast_construct_is_empty()));
		result.push_back(s_ptr<ast_construct>(new ast_construct_head()));
		result.push_back(s_ptr<ast_construct>(new ast_construct_tail()));
		result.push_back(s_ptr<ast_construct>(new ast_construct_fst()));
		result.push_back(s_ptr<ast_construct>(new ast_construct_snd()));
		
		return result;
	}
	
	void ast_prog::add_decl(s_ptr<ast_decl> decl)
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
		//Register constructs
		for(const s_ptr<ast_construct> cons : constrs)
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
	
	s_ptr<const ir_stmt> ast_prog::translate(ircontext& c) const
	{
		const ir_label l_start = c.create_label();
		const ir_label l_end = c.create_label();
		
		s_ptr<const ir_stmt> r(ir_stmt_label::create(l_start));
		
		//Create labels for vars and funs, store in context and in seperate 
		std::map<sid, ir_label> labelmap;
		for(const auto decl : decls)
		{
			const sid sid_decl = decl->fetch_id();
			const ir_label l_decl = c.create_label();
			
			labelmap[sid_decl] = l_decl;
			c.register_memloc(sid_decl, ir_exp_name::create(l_decl));
		}
		
		for(const auto cons : constrs)
		{
			const sid sid_cons = cons->fetch_id();
			const ir_label l_cons = c.create_label();
			
			labelmap[sid_cons] = l_cons;
			c.register_memloc(sid_cons, ir_exp_name::create(l_cons));
		}
		
		//Initialize registers
		ir_stmt::cat(r, ir_stmt_move::create(ir_exp_temp::create(c.heap_reg), ir_exp_name::create(c.l_heap)));
		ir_stmt::cat(r, ir_stmt_move::create(ir_exp_temp::create(c.stack_reg), ir_exp_name::create(c.l_stack)));
		ir_stmt::cat(r, ir_stmt_move::create(ir_exp_temp::create(c.frame_reg), ir_exp_name::create(c.l_stack)));
		
		//Translate code for initializing and construction global variables
		//TODO Variables might be dependant on each other: dependency graph, initialize in proper order
		for(const auto decl : decls)
			if(decl->type() == ast_decl::t_var_decl)
				ir_stmt::cat(r, std::dynamic_pointer_cast<ast_decl_var>(decl)->v->translate(c));
		
		//Translate code for language constructs
		for(const auto cons : constrs)
			ir_stmt::cat(r, cons->translate(labelmap.at(cons->fetch_id()), c));
		
		//Translate code for functions
		for(const auto decl : decls)
			if(decl->type() == ast_decl::t_fun_decl)
				ir_stmt::cat(r, std::dynamic_pointer_cast<ast_decl_fun>(decl)->f->translate(labelmap.at(decl->fetch_id()), c));
		
		//TODO Call main
		
		ir_stmt::cat(r, ir_stmt_label::create(l_end));
		
		//TODO HALT
		
		for(const auto decl : decls)
			if(decl->type() == ast_decl::t_var_decl)
			{
				//TODO: Force allocation of memory
				ir_stmt::cat(r, ir_stmt_label::create(labelmap.at(decl->fetch_id())));
			}
		
		return r;
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

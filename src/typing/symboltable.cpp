#include "symboltable.hpp"

#include <map>

#include "typecontext.hpp"
#include "ltypecontext.hpp"

#include "types/sl_polytype.hpp"
#include "types/sl_type_unbound.hpp"

namespace splicpp
{
	sid symboltable::create_entry(const symbolref::symbolreftype t, const size_t i)
	{
		sid j = index.size();
		index.push_back(symbolref(t, i));
		return j;
	}
	
	std::vector<sid> symboltable::select_all(const symbolref::symbolreftype t) const
	{
		std::vector<sid> result;
		for(sid i = 0; i < index.size(); i++)
			if(index[i].t == t)
				result.push_back(i);
		
		return result;
	}
	
	sid symboltable::reg_fun(std::shared_ptr<ast_fun_decl> f)
	{
		sid i = create_entry(symbolref::t_fun, funs.size());
		f->assign(i);
		funs.push_back(f);
		return i;
	}
	
	sid symboltable::reg_cons(std::shared_ptr<ast_construct> cons)
	{
		sid i = create_entry(symbolref::t_construct, conss.size());
		cons->assign(i);
		conss.push_back(cons);
		return i;
	}	
	
	sid symboltable::reg_var(std::shared_ptr<ast_var_decl> v)
	{
		sid i = create_entry(symbolref::t_var, vars.size());
		v->assign(i);
		vars.push_back(v);
		return i;
	}
	
	sid symboltable::reg_arg(std::shared_ptr<ast_f_arg> a)
	{
		sid i = create_entry(symbolref::t_arg, args.size());
		a->assign(i);
		args.push_back(a);
		return i;
	}
	
	sid symboltable::reg_lvar(std::shared_ptr<ast_var_decl> lv)
	{
		sid i = create_entry(symbolref::t_local_var, local_vars.size());
		lv->assign(i);
		local_vars.push_back(lv);
		return i;
	}
	
	sid symboltable::reg_type(std::shared_ptr<ast_type_id> t)
	{
		sid i = create_entry(symbolref::t_type, types.size());
		t->assign(i);
		types.push_back(t);
		return i;
	}
	
	void symboltable::check_types() const
	{
		std::shared_ptr<typecontext> global(new typecontext());
		ltypecontext c(global);
		substitution s;
		
		std::map<sid, std::shared_ptr<sl_type_unbound>> init_types;
		for(size_t i = 0; i < index.size(); i++)
			if(index[i].t != symbolref::t_type && index[i].t != symbolref::t_arg && index[i].t != symbolref::t_local_var)
			{
				const std::shared_ptr<sl_type_unbound> t = c.create_fresh();
				init_types[i] = t;
				c.register_type(i, sl_polytype::qualify(c, t));
			}
		
		print(c, std::cout << std::endl << "Typecontext initial: " << std::endl);
		
		for(const sid i : select_all(symbolref::symbolreftype::t_construct))
		{
			c = c.apply(s);
			std::shared_ptr<sl_type_unbound> t = init_types[i];
			s = conss[index[i].i]->declare_type(c).composite(s);
			//s = init_types[i]->apply(s)->unify(c[i]->apply(c, s)->unbind_maintain()).composite(s);
		}
		
		//s.print(std::cout << std::endl << "Substitutions: " << std::endl);
		
		for(const sid i : select_all(symbolref::symbolreftype::t_var))
		{
			c = c.apply(s);
			std::shared_ptr<sl_type_unbound> t = init_types[i];
			s = vars[index[i].i]->declare_type(c).composite(s);
			//s = init_types[i]->apply(s)->unify(c[i]->apply(c, s)->unbind_maintain()).composite(s);
		}
		
		for(const sid i : select_all(symbolref::symbolreftype::t_fun))
		{
			c = c.apply(s);
			s = funs[index[i].i]->declare_type(c).composite(s);
			//c = c.apply(s);
			
			//s = init_types[i]->apply(s)->unify(c[i]->apply(c, s)->unbind_maintain()).composite(s);
		}
		
		//for(const sid i : select_all(symbolref::symbolreftype::t_local_var))
		//	s = local_vars[index[i].i]->infer_type(c, init_types[i]).composite(s);
		
		/*
		for(const sid i : select_all(symbolref::symbolreftype::t_type))
			c.register_type(i, std::static_pointer_cast<sl_type>(c.create_fresh()));
		
		for(const sid i : select_all(symbolref::symbolreftype::t_var))
			c.register_type(i, vars[index[i].i]->fetch_assigned_type(c));
		
		for(const sid i : select_all(symbolref::symbolreftype::t_fun))
			c.register_type(i, funs[index[i].i]->fetch_assigned_type(c));
		
		for(const sid i : select_all(symbolref::symbolreftype::t_arg))
			c.register_type(i, args[index[i].i]->fetch_assigned_type(c));
		
		for(const sid i : select_all(symbolref::symbolreftype::t_local_var))
			c.register_type(i, local_vars[index[i].i]->fetch_assigned_type(c));
			
		//Set inferred types
		for(const sid i : select_all(symbolref::symbolreftype::t_var))
			c.register_type(i, c[i]->apply(vars[index[i].i]->infer_type(c)));
		*/
		
		/*for(const sid i : select_all(symbolref::symbolreftype::t_var))
			c[i]->unify(vars[index[i].i]->fetch_assigned_type(c));
		
		for(const sid i : select_all(symbolref::symbolreftype::t_fun))
			c[i]->unify(funs[index[i].i]->fetch_assigned_type(c));
		*/
		s.print(std::cout << std::endl << "Substitutions: " << std::endl);
		
		print(global->apply_maintain(substitution::id()), std::cout << std::endl << "Typecontext pre-final: " << std::endl);
		print(global->apply_maintain(s), std::cout << std::endl << "Typecontext final: " << std::endl);
	}
	
	void symboltable::print(std::ostream& s) const
	{
		for(sid i = 0; i < index.size(); i++)
		{
			print_name(i, s << i << ": ");
			s << std::endl;
		}
	}
	
	void symboltable::print(const typecontext& c, std::ostream& s) const
	{
		for(sid i = 0; i < index.size(); i++)
		{
			print_name(i, s << i << ": ");
			
			s << " = ";
			
			if(c.is_registered(i))
				c[i]->print(s);
			else
				s << "undef";
				
			s << std::endl;
		}
	}
	
	void symboltable::print_name(const sid i, std::ostream& s) const
	{
		switch(index[i].t)
		{
			case symbolref::t_fun:
				s << funs.at(index[i].i)->fetch_name();
				break;
			case symbolref::t_construct:
				s << conss.at(index[i].i)->fetch_name();
				break;
			case symbolref::t_var:
				s << vars.at(index[i].i)->fetch_name();
				break;
			case symbolref::t_arg:
				s << args.at(index[i].i)->fetch_name();
				break;
			case symbolref::t_local_var:
				s << local_vars.at(index[i].i)->fetch_name();
				break;
			case symbolref::t_type:
				s << types.at(index[i].i)->fetch_name();
				break;
		}
	}
}

#include "symboltable.hpp"

#include <map>
#include <stdexcept>

#include "../common/generic.hpp"

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
	
	sid symboltable::reg_fun(s_ptr<ast_fun_decl> f)
	{
		sid i = create_entry(symbolref::t_fun, funs.size());
		f->assign(i);
		funs.push_back(f);
		return i;
	}
	
	sid symboltable::reg_cons(s_ptr<ast_construct> cons)
	{
		sid i = create_entry(symbolref::t_construct, conss.size());
		cons->assign(i);
		conss.push_back(cons);
		return i;
	}	
	
	sid symboltable::reg_var(s_ptr<ast_var_decl> v)
	{
		sid i = create_entry(symbolref::t_var, vars.size());
		v->assign(i);
		vars.push_back(v);
		return i;
	}
	
	sid symboltable::reg_arg(s_ptr<ast_f_arg> a)
	{
		sid i = create_entry(symbolref::t_arg, args.size());
		a->assign(i);
		args.push_back(a);
		return i;
	}
	
	sid symboltable::reg_lvar(s_ptr<ast_var_decl> lv)
	{
		sid i = create_entry(symbolref::t_local_var, local_vars.size());
		lv->assign(i);
		local_vars.push_back(lv);
		return i;
	}
	
	sid symboltable::reg_type(s_ptr<ast_type_id> t)
	{
		sid i = create_entry(symbolref::t_type, types.size());
		t->assign(i);
		types.push_back(t);
		return i;
	}
	
	void symboltable::check_types() const
	{
		s_ptr<typecontext> global(new typecontext());
		ltypecontext c(global);
		substitution s;
		
		std::map<sid, s_ptr<const sl_polytype_exists>> init_types;
		for(size_t i = 0; i < index.size(); i++)
			if(index[i].t == symbolref::t_var || index[i].t == symbolref::t_fun)
			{
				const s_ptr<const sl_polytype_exists> t(new sl_polytype_exists());
				init_types[i] = t;
				c.register_type(i, std::static_pointer_cast<const sl_polytype>(t));
			}
		
		for(const sid i : select_all(symbolref::symbolreftype::t_construct))
			s = conss[index[i].i]->declare_type(c).composite(s);
		
		ltypecontext corig = c;
		
		for(const sid i : select_all(symbolref::symbolreftype::t_var))
		{
			ltypecontext ctmp = corig;
			s = vars[index[i].i]->declare_type(ctmp).composite(s);
			c.register_type(i, ctmp[i]); //Copy result from ctmp to c;
		}
		
		for(const sid i : select_all(symbolref::symbolreftype::t_fun))
		{
			ltypecontext ctmp = corig;
			s = funs[index[i].i]->declare_type(ctmp).composite(s);
			c.register_type(i, ctmp[i]); //Copy result from ctmp to c;
		}
		
		std::vector<sid> prop_index;
		for(const std::pair<sid, s_ptr<const sl_polytype_exists>> p : init_types)
			prop_index.push_back(p.first);
		
		while(!prop_index.empty())
		{
			bool changed = false;
			
			for(size_t i = 0; i < prop_index.size(); i++)
			{
				const s_ptr<const sl_polytype_exists> t = init_types[prop_index[i]];
				
				std::vector<s_ptr<const sl_type_unbound>> blacklist;
				for(size_t j = 0; j < prop_index.size(); j++)
					if(i != j)
						for(const auto tmp : init_types[prop_index[j]]->fetch_bindings())
							add_to<s_ptr<const sl_type_unbound>>(tmp->apply(s)->tv(), blacklist);
				
				if(any_is_in_ptr<const sl_type_unbound>(std::dynamic_pointer_cast<const sl_polytype_forall>(c[prop_index[i]])->unbind_maintain()->apply(s)->tv(), blacklist))
					continue;
	
				s = t->propagate_findings(c, c[prop_index[i]]->apply(c, s), s);
				
				prop_index.erase(prop_index.begin()+i);
				i--;
				
				changed = true;
				break;
			}
			
			if(!prop_index.empty() && !changed)
			{
				for(size_t i = 0; i < prop_index.size(); i++)
				{
					print_name(prop_index[i], std::cout << std::endl);
					for(const auto t : std::dynamic_pointer_cast<const sl_polytype_forall>(c[prop_index[i]])->unbind_maintain()->apply(s)->tv())
						t->print(std::cout << ' ');
				}
				throw std::runtime_error("Cyclic dependency in definitions, cannot infer type");
			}
		}
		
		for(const sid i : select_all(symbolref::symbolreftype::t_type))
			c.register_type(i, sl_polytype::not_qualify(c.create_fresh(sloc())));
		
		typecontext gtmp = global->apply_maintain(s);
		substitution stmp;
		
		for(const sid i : select_all(symbolref::symbolreftype::t_var))
		{
			gtmp = gtmp.apply(stmp);
			stmp = vars[index[i].i]->fetch_assigned_type(gtmp)->unify(gtmp[i]->unbind(gtmp)).composite(stmp);
		}
		
		for(const sid i : select_all(symbolref::symbolreftype::t_fun))
		{
			gtmp = gtmp.apply(stmp);
			stmp = funs[index[i].i]->fetch_assigned_type(gtmp)->unify(gtmp[i]->unbind(gtmp)).composite(stmp);
		}
		
		for(const sid i : select_all(symbolref::symbolreftype::t_arg))
		{
			gtmp = gtmp.apply(stmp);
			stmp = args[index[i].i]->fetch_assigned_type(gtmp)->unify(gtmp[i]->unbind(gtmp)).composite(stmp);
		}
		
		for(const sid i : select_all(symbolref::symbolreftype::t_local_var))
		{
			gtmp = gtmp.apply(stmp);
			stmp = local_vars[index[i].i]->fetch_assigned_type(gtmp)->unify(gtmp[i]->unbind(gtmp)).composite(stmp);
		}
		
		print(global->apply_maintain(stmp.composite(s)), std::cout << "Typecontext final: " << std::endl);
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

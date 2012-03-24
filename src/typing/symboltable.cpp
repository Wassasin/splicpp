#include "symboltable.hpp"

namespace splicpp
{
	sid symboltable::create_entry(symbolref::symbolreftype t, size_t i)
	{
		sid j = index.size();
		index.push_back(symbolref(t, i));
		return j;
	}
	
	sid symboltable::reg_fun(std::shared_ptr<ast_fun_decl> f)
	{
		sid i = create_entry(symbolref::t_fun, funs.size());
		f->assign(i);
		funs.push_back(f);
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
	
	void symboltable::print(std::ostream& s) const
	{
		for(sid i = 0; i < index.size(); i++)
		{
			s << i << ": ";
			switch(index[i].t)
			{
				case symbolref::t_fun:
					s << funs.at(index[i].i)->fetch_name();
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
			s << std::endl;
		}
	}
}

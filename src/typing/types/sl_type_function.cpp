#include "sl_type_function.hpp"

#include <sstream>

#include "../../common/generic.hpp"
#include "../../common/utils.hpp"
#include "../unification_error.hpp"

#include "sl_type_unbound.hpp"

namespace splicpp
{	
	sl_type::sl_type_type sl_type_function::type() const
	{
		return t_function;
	}
	
	void sl_type_function::print(std::ostream& s) const
	{
		if(args.size() > 0)
		{
			s << "<";
			delim_printer p(", ", s);
			for(const auto arg : args)
			{
				std::stringstream stmp;
				arg->print(stmp);
				p.print(stmp.str());
			}
			s << "> -> ";
		}
		
		r->print(s);
	}
	
	std::vector<std::shared_ptr<sl_type_unbound>> sl_type_function::tv() const
	{
		std::vector<std::shared_ptr<sl_type_unbound>> result;
		for(const auto arg : args)
			for(const auto u : arg->tv())
				if(!is_in_ptr<sl_type_unbound>(u, result))
					result.push_back(u);
		
		for(const auto u : r->tv())
			if(!is_in_ptr<sl_type_unbound>(u, result))
				result.push_back(u);
		
		return result;
	}
	
	substitution sl_type_function::unify_partial(const std::shared_ptr<sl_type> t) const
	{
		if(t->type() != t_function)
			throw unification_error(this, t.get());
		
		std::shared_ptr<sl_type_function> tf = std::dynamic_pointer_cast<sl_type_function>(t);
		
		if(args.size() != tf->args.size())
			throw unification_error(this, tf.get());
			
		substitution s;
		for(size_t i = 0; i < args.size(); i++)
			s = s.composite(args[i]->unify(tf->args[i]));
		
		return s.composite(r->unify(tf->r));
	}
	
	std::shared_ptr<sl_type> sl_type_function::apply(const substitution& s) const
	{
		std::vector<std::shared_ptr<sl_type>> newargs;
		for(const auto arg : args)
			newargs.push_back(arg->apply(s));
		
		return std::shared_ptr<sl_type>(new sl_type_function(newargs, r->apply(s)));
	}
}

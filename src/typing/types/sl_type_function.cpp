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
		s << "<";
		
		if(args.size() > 0)
		{
			delim_printer p(", ", s);
			for(const auto arg : args)
			{
				std::stringstream stmp;
				arg->print(stmp);
				p.print(stmp.str());
			}
		}
		
		s << "> -> ";
		r->print(s);
	}
	
	std::vector<cs_ptr<sl_type_unbound>> sl_type_function::tv() const
	{
		std::vector<cs_ptr<sl_type_unbound>> result;
		for(const auto arg : args)
			for(const auto u : arg->tv())
				if(!is_in_ptr<const sl_type_unbound>(u, result))
					result.push_back(u);
		
		for(const auto u : r->tv())
			if(!is_in_ptr<const sl_type_unbound>(u, result))
				result.push_back(u);
		
		return result;
	}
	
	boost::optional<substitution> sl_type_function::unify_partial(const cs_ptr<sl_type> t) const
	{
		if(t->type() != t_function)
			return boost::optional<substitution>();
		
		cs_ptr<sl_type_function> tf = std::dynamic_pointer_cast<const sl_type_function>(t);
		
		if(args.size() != tf->args.size())
			return boost::optional<substitution>();
			
		auto stmp = r->unify_internal(tf->r);
		if(!stmp)
			return boost::optional<substitution>();
		
		substitution s = stmp.get();
		for(size_t i = 0; i < args.size(); i++)
		{
			stmp = args.at(args.size()-(i+1))->apply(s)->unify_internal(tf->args.at(args.size()-(i+1))->apply(s));
			if(!stmp)
				return boost::optional<substitution>();
			
			s = stmp.get().composite(s);
		}
		
		return s;
	}
	
	cs_ptr<sl_type> sl_type_function::apply(const substitution& s) const
	{
		std::vector<cs_ptr<sl_type>> newargs;
		for(const auto arg : args)
			newargs.push_back(arg->apply(s));
		
		return cs_ptr<sl_type>(new sl_type_function(newargs, r->apply(s)));
	}
}

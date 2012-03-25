#include "sl_type_function.hpp"

#include <sstream>

#include "../../common/utils.hpp"

namespace splicpp
{	
	sl_type::sl_type_type sl_type_function::type() const
	{
		return t_function;
	}
	
	void sl_type_function::print(std::ostream& s) const
	{
		delim_printer p(", ", s);
		s << "<";
		
		for(const auto arg : args)
		{
			std::stringstream stmp;
			arg->print(stmp);
			p.print(stmp.str());
		}
		
		s << "> -> ";
		r->print(s);
	}
	
	std::vector<std::shared_ptr<sl_type_unbound>> sl_type_function::fv() const
	{
		std::vector<std::shared_ptr<sl_type_unbound>> result;
		for(const auto arg : args)
			for(const auto u : arg->fv())
				result.push_back(u);
		
		for(const auto u : r->fv())
			result.push_back(u);
		
		return result;
	}
	
	//substitution sl_type_array::unify(const std::shared_ptr<sl_type> t, typecontext& c) const; //TODO
	std::shared_ptr<sl_type> sl_type_function::apply(const substitution& s) const
	{
		std::vector<std::shared_ptr<sl_type>> newargs;
		for(const auto arg : args)
			newargs.push_back(arg->apply(s));
		
		return std::shared_ptr<sl_type>(new sl_type_function(newargs, r->apply(s)));
	}
}

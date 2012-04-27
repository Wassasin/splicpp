#include "sl_type_int.hpp"

#include "../unification_error.hpp"

namespace splicpp
{	
	sl_type::sl_type_type sl_type_int::type() const
	{
		return t_int;
	}
	
	void sl_type_int::print(std::ostream& s) const
	{
		s << "Int";
	}
	
	std::vector<cs_ptr<sl_type_unbound>> sl_type_int::tv() const
	{
		return std::vector<cs_ptr<sl_type_unbound>>();
	}
	
	boost::optional<substitution> sl_type_int::unify_partial(const cs_ptr<sl_type> t) const
	{
		if(t->type() != t_int)
			return boost::optional<substitution>();
			
		return substitution::id();
	}
	
	cs_ptr<sl_type> sl_type_int::apply(const substitution&) const
	{
		return shared_from_this();
	}
}

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
	
	std::vector<s_ptr<const sl_type_unbound>> sl_type_int::tv() const
	{
		return std::vector<s_ptr<const sl_type_unbound>>();
	}
	
	sl_type::unify_details sl_type_int::unify_partial(const s_ptr<const sl_type> t) const
	{
		if(t->type() != t_int)
			return unify_details(shared_from_this(), t);
			
		return substitution::id();
	}
	
	s_ptr<const sl_type> sl_type_int::apply(const substitution&) const
	{
		return shared_from_this();
	}
}

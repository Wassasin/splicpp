#include "sl_type_bool.hpp"

#include "../unification_error.hpp"

namespace splicpp
{	
	sl_type::sl_type_type sl_type_bool::type() const
	{
		return t_bool;
	}
	
	void sl_type_bool::print(std::ostream& s) const
	{
		s << "Bool";
	}
	
	std::vector<s_ptr<const sl_type_unbound>> sl_type_bool::tv() const
	{
		return std::vector<s_ptr<const sl_type_unbound>>();
	}
	
	sl_type::unify_details sl_type_bool::unify_partial(const s_ptr<const sl_type> t) const
	{
		if(t->type() != t_bool)
			return unify_details(shared_from_this(), t);
		
		return substitution::id();
	}
	
	s_ptr<const sl_type> sl_type_bool::apply(const substitution&) const
	{
		return shared_from_this();
	}
}

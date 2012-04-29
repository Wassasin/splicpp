#include "sl_type_void.hpp"

#include "../unification_error.hpp"

namespace splicpp
{	
	sl_type::sl_type_type sl_type_void::type() const
	{
		return t_void;
	}
	
	void sl_type_void::print(std::ostream& s) const
	{
		s << "Void";
	}
	
	std::vector<s_ptr<const sl_type_unbound>> sl_type_void::tv() const
	{
		return std::vector<s_ptr<const sl_type_unbound>>();
	}
	
	sl_type::unify_details sl_type_void::unify_partial(const s_ptr<const sl_type> t) const
	{
		if(t->type() != t_void)
			return sl_type::unify_details(shared_from_this(), t);
		
		return substitution::id();
	}
	
	s_ptr<const sl_type> sl_type_void::apply(const substitution&) const
	{
		return shared_from_this();
	}
}

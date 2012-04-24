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
	
	std::vector<s_ptr<sl_type_unbound>> sl_type_bool::tv() const
	{
		return std::vector<s_ptr<sl_type_unbound>>();
	}
	
	boost::optional<substitution> sl_type_bool::unify_partial(const s_ptr<sl_type> t) const
	{
		if(t->type() != t_bool)
			return boost::optional<substitution>();
		
		return substitution::id();
	}
	
	s_ptr<sl_type> sl_type_bool::apply(const substitution&) const
	{
		return s_ptr<sl_type>(new sl_type_bool());
	}
}

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
	
	std::vector<s_ptr<sl_type_unbound>> sl_type_int::tv() const
	{
		return std::vector<s_ptr<sl_type_unbound>>();
	}
	
	boost::optional<substitution> sl_type_int::unify_partial(const s_ptr<sl_type> t) const
	{
		if(t->type() != t_int)
			return boost::optional<substitution>();
			
		return substitution::id();
	}
	
	s_ptr<sl_type> sl_type_int::apply(const substitution&) const
	{
		return s_ptr<sl_type>(new sl_type_int());
	}
}

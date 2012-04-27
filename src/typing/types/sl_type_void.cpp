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
	
	std::vector<cs_ptr<sl_type_unbound>> sl_type_void::tv() const
	{
		return std::vector<cs_ptr<sl_type_unbound>>();
	}
	
	boost::optional<substitution> sl_type_void::unify_partial(const cs_ptr<sl_type> t) const
	{
		if(t->type() != t_void)
			return boost::optional<substitution>();
		
		return substitution::id();
	}
	
	cs_ptr<sl_type> sl_type_void::apply(const substitution&) const
	{
		return shared_from_this();
	}
}

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
	
	std::vector<std::shared_ptr<sl_type_unbound>> sl_type_void::tv() const
	{
		return std::vector<std::shared_ptr<sl_type_unbound>>();
	}
	
	substitution sl_type_void::unify(const std::shared_ptr<sl_type> t) const
	{
		if(t->type() != t_void)
			throw unification_error(this, t.get());
		
		return substitution::id();
	}
	
	std::shared_ptr<sl_type> sl_type_void::apply(const substitution&) const
	{
		return std::shared_ptr<sl_type>(new sl_type_void());
	}
}

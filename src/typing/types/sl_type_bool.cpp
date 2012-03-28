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
	
	std::vector<std::shared_ptr<sl_type_unbound>> sl_type_bool::tv() const
	{
		return std::vector<std::shared_ptr<sl_type_unbound>>();
	}
	
	substitution sl_type_bool::unify(const std::shared_ptr<sl_type> t, typecontext&) const
	{
		if(t->type() != t_bool)
			throw unification_error(this, t.get());
		
		return substitution::id();
	}
	
	std::shared_ptr<sl_type> sl_type_bool::apply(const substitution&) const
	{
		return std::shared_ptr<sl_type>(new sl_type_bool());
	}
}

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
	
	std::vector<std::shared_ptr<sl_type_unbound>> sl_type_int::tv() const
	{
		return std::vector<std::shared_ptr<sl_type_unbound>>();
	}
	
	substitution sl_type_int::unify(const std::shared_ptr<sl_type> t, typecontext&) const
	{
		if(t->type() != t_int)
			throw unification_error(this, t.get());
			
		return substitution::id();
	}
	
	std::shared_ptr<sl_type> sl_type_int::apply(const substitution&) const
	{
		return std::shared_ptr<sl_type>(new sl_type_int());
	}
}

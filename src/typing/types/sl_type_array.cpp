#include "sl_type_array.hpp"

#include "../unification_error.hpp"

namespace splicpp
{	
	sl_type::sl_type_type sl_type_array::type() const
	{
		return t_array;
	}
	
	void sl_type_array::print(std::ostream& s) const
	{
		s << '[';
		t->print(s);
		s << ']';
	}
	
	std::vector<s_ptr<const sl_type_unbound>> sl_type_array::tv() const
	{
		return t->tv();
	}
	
	sl_type::unify_details sl_type_array::unify_partial(const s_ptr<const sl_type> t) const
	{
		if(t->type() != t_array)
			return unify_details(shared_from_this(), t);
		
		return this->t->unify_internal(std::dynamic_pointer_cast<const sl_type_array>(t)->t);
	}
	
	s_ptr<const sl_type> sl_type_array::apply(const substitution& s) const
	{
		return make_s<sl_type_array>(t->apply(s), sl);
	}
}

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
	
	std::vector<s_ptr<sl_type_unbound>> sl_type_array::tv() const
	{
		return t->tv();
	}
	
	boost::optional<substitution> sl_type_array::unify_partial(const s_ptr<sl_type> t) const
	{
		if(t->type() != t_array)
			return boost::optional<substitution>();
		
		return this->t->unify_internal(std::dynamic_pointer_cast<sl_type_array>(t)->t);
	}
	
	s_ptr<sl_type> sl_type_array::apply(const substitution& s) const
	{
		return s_ptr<sl_type>(new sl_type_array(t->apply(s)));
	}
}

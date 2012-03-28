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
	
	std::vector<std::shared_ptr<sl_type_unbound>> sl_type_array::tv() const
	{
		return t->tv();
	}
	
	substitution sl_type_array::unify(const std::shared_ptr<sl_type> t, typecontext& c) const
	{
		if(t->type() != t_array)
			throw unification_error(this, t.get());
	
		return this->t->unify(std::dynamic_pointer_cast<sl_type_array>(t)->t, c);
	}
	
	std::shared_ptr<sl_type> sl_type_array::apply(const substitution& s) const
	{
		return std::shared_ptr<sl_type>(new sl_type_array(t->apply(s)));
	}
}

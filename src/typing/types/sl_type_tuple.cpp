#include "sl_type_tuple.hpp"

namespace splicpp
{	
	sl_type::sl_type_type sl_type_tuple::type() const
	{
		return t_tuple;
	}
	
	void sl_type_tuple::print(std::ostream& s) const
	{
		s << '(';
		t_left->print(s);
		s << ", ";
		t_right->print(s);
		s << ')';
	}
	
	std::vector<std::shared_ptr<sl_type_unbound>> sl_type_tuple::tv() const
	{
		auto result = t_left->tv();
		for(const auto x : t_right->tv())
			result.push_back(x);
		
		return result;
	}
	
	//substitution sl_type_tuple::unify(const std::shared_ptr<sl_type> t, typecontext& c) const; //TODO
	std::shared_ptr<sl_type> sl_type_tuple::apply(const substitution& s) const
	{
		return std::shared_ptr<sl_type>(new sl_type_tuple(t_left->apply(s), t_right->apply(s)));
	}
}

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
	
	std::vector<std::shared_ptr<sl_type_unbound>> sl_type_tuple::fv() const
	{
		auto result = t_left->fv();
		for(const auto x : t_right->fv())
			result.push_back(x);
		
		return result;
	}
	
	//substitution sl_type_array::unify(const std::shared_ptr<sl_type> t, typecontext& c) const; //TODO
	std::shared_ptr<sl_type> sl_type_tuple::apply(const substitution& s) const
	{
		return std::shared_ptr<sl_type>(new sl_type_tuple(t_left->apply(s), t_right->apply(s)));
	}
}

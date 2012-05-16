#include "sl_type_tuple.hpp"

#include "../../common/generic.hpp"
#include "../unification_error.hpp"

#include "sl_type_unbound.hpp"

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
	
	std::vector<s_ptr<const sl_type_unbound>> sl_type_tuple::tv() const
	{
		auto result = t_left->tv();
		for(const auto x : t_right->tv())
			if(!is_in_ptr<const sl_type_unbound>(x, result))
				result.push_back(x);
		
		return result;
	}
	
	sl_type::unify_details sl_type_tuple::unify_partial(const s_ptr<const sl_type> t) const
	{
		if(t->type() != t_tuple)
			return unify_details(shared_from_this(), t);
		
		s_ptr<const sl_type_tuple> tt = std::dynamic_pointer_cast<const sl_type_tuple>(t);
		const unify_details srighttmp = t_right->unify_internal(tt->t_right);
		if(!srighttmp.is_success())
			return srighttmp;
		
		const substitution sright = srighttmp.fetch_s();
		const unify_details slefttmp = t_left->apply(sright)->unify_internal(tt->t_left->apply(sright));
		
		if(!slefttmp.is_success())
			return slefttmp;
		
		return slefttmp.fetch_s().composite(sright);
	}
	
	s_ptr<const sl_type> sl_type_tuple::apply(const substitution& s) const
	{
		return make_s<sl_type_tuple>(t_left->apply(s), t_right->apply(s), sl);
	}
}

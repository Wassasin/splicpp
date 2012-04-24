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
	
	std::vector<cs_ptr<sl_type_unbound>> sl_type_tuple::tv() const
	{
		auto result = t_left->tv();
		for(const auto x : t_right->tv())
			if(!is_in_ptr<const sl_type_unbound>(x, result))
				result.push_back(x);
		
		return result;
	}
	
	boost::optional<substitution> sl_type_tuple::unify_partial(const cs_ptr<sl_type> t) const
	{
		if(t->type() != t_tuple)
			return boost::optional<substitution>();
		
		cs_ptr<sl_type_tuple> tt = std::dynamic_pointer_cast<const sl_type_tuple>(t);
		const auto srighttmp = t_right->unify_internal(tt->t_right);
		if(!srighttmp)
			return boost::optional<substitution>();
		
		const auto sright = srighttmp.get();
		const auto slefttmp = t_left->apply(sright)->unify_internal(tt->t_left->apply(sright));
		
		if(!slefttmp)
			return boost::optional<substitution>();
		
		return slefttmp.get().composite(sright);
	}
	
	cs_ptr<sl_type> sl_type_tuple::apply(const substitution& s) const
	{
		return cs_ptr<sl_type>(new sl_type_tuple(t_left->apply(s), t_right->apply(s)));
	}
}

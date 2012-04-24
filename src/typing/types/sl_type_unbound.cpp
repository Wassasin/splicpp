#include "sl_type_unbound.hpp"

#include <iostream>

#include "sl_type.hpp"
#include "../unification_error.hpp"

namespace splicpp
{
	bool sl_type_unbound::is_unbound() const
	{
		return true;
	}
	
	sl_type::sl_type_type sl_type_unbound::type() const
	{
		return t_unbound;
	}
	
	bool sl_type_unbound::equals(const s_ptr<sl_type_unbound> y) const
	{
		return(id == y->id);
	}
	
	bool sl_type_unbound::operator<(const s_ptr<sl_type_unbound> y) const
	{
		return(id < y->id);
	}
	
	bool sl_type_unbound::operator==(const s_ptr<sl_type_unbound> y) const
	{
		return equals(y);
	}
	
	void sl_type_unbound::print(std::ostream& s) const
	{
		s << "t" << id;
	}
	
	boost::optional<substitution> sl_type_unbound::unify_partial(const s_ptr<sl_type> t) const
	{
		const s_ptr<sl_type_unbound> copy(new sl_type_unbound(*this));
		if(t->type() == t_unbound && equals(std::dynamic_pointer_cast<sl_type_unbound>(t)))
			return substitution::id();
	
		for(const auto tx : t->tv())
			if(copy->equals(tx))
				throw boost::optional<substitution>();
		
		substitution s;
		s.add(copy, t);
		return s;
	}
	
	std::vector<s_ptr<sl_type_unbound>> sl_type_unbound::tv() const
	{
		std::vector<s_ptr<sl_type_unbound>> result;
		result.push_back(s_ptr<sl_type_unbound>(new sl_type_unbound(*this)));
		return result;
	}
	
	s_ptr<sl_type> sl_type_unbound::apply(const substitution& s) const
	{
		return s.substitute(s_ptr<sl_type_unbound>(new sl_type_unbound(*this)));
	}
}

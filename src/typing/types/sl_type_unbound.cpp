#include "sl_type_unbound.hpp"

#include "sl_type.hpp"

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
	
	bool sl_type_unbound::equals(const std::shared_ptr<sl_type_unbound> y) const
	{
		return(id == y->id);
	}
	
	void sl_type_unbound::print(std::ostream& s) const
	{
		s << "t" << id;
	}
	
	std::vector<std::shared_ptr<sl_type_unbound>> sl_type_unbound::fv() const
	{
		std::vector<std::shared_ptr<sl_type_unbound>> result;
		result.push_back(std::shared_ptr<sl_type_unbound>(new sl_type_unbound(*this)));
		return result;
	}
	
	std::shared_ptr<sl_type> sl_type_unbound::apply(const substitution& s) const
	{
		return s.substitute(std::shared_ptr<sl_type_unbound>(new sl_type_unbound(*this)));
	}
}

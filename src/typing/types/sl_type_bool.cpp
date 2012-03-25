#include "sl_type_bool.hpp"

namespace splicpp
{	
	sl_type::sl_type_type sl_type_bool::type() const
	{
		return t_bool;
	}
	
	void sl_type_bool::print(std::ostream& s) const
	{
		s << "Bool";
	}
	
	std::vector<std::shared_ptr<sl_type_unbound>> sl_type_bool::fv() const
	{
		return std::vector<std::shared_ptr<sl_type_unbound>>();
	}
	
	//substitution sl_type_bool::unify(const std::shared_ptr<sl_type> t, typecontext& c) const; //TODO
	std::shared_ptr<sl_type> sl_type_bool::apply(const substitution&) const
	{
		return std::shared_ptr<sl_type>(new sl_type_bool());
	}
}

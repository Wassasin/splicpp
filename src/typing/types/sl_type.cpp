#include "sl_type.hpp"

#include "../../common/generic.hpp"
#include "../typecontext.hpp"
#include "../substitution.hpp"

#include "sl_type_universal.hpp"
#include "sl_type_unbound.hpp"

namespace splicpp
{
	substitution sl_type::unify(const std::shared_ptr<sl_type> t) const
	{
		if(t->type() == t_unbound)
			return t->unify_partial(apply(substitution::id()));
		else
			return unify_partial(t);
	}
	
	std::shared_ptr<sl_type> sl_type::qualify(const typecontext& c) const
	{
		const auto b = subtract_ptr<sl_type_unbound>(tv(), c.fv());
		if(b.size() == 0)
			return apply(substitution::id()); //Hack to return this
		
		std::shared_ptr<sl_type_universal> result(new sl_type_universal(apply(substitution::id())));
		for(const auto x : b)
			result->bind(x);
		
		return std::static_pointer_cast<sl_type>(result);
	}
	
	std::shared_ptr<sl_type> sl_type::force_qualify() const
	{
		const auto b = tv();
		if(b.size() == 0)
			return apply(substitution::id()); //Hack to return this
		
		std::shared_ptr<sl_type_universal> result(new sl_type_universal(apply(substitution::id())));
		for(const auto x : b)
			result->bind(x);
		
		return std::static_pointer_cast<sl_type>(result);
	}

	bool sl_type::is_unbound() const
	{
		return false;
	}
}

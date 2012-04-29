#include "sl_type.hpp"

#include "../../common/generic.hpp"
#include "../typecontext.hpp"
#include "../substitution.hpp"
#include "../unification_error.hpp"

#include "sl_type_unbound.hpp"

namespace splicpp
{
	sl_type::unify_details sl_type::unify_internal(const s_ptr<const sl_type> t) const
	{
		const s_ptr<const sl_type> x = shared_from_this();
		const s_ptr<const sl_type> y = t;
		
		const unify_details u = x->unify_partial(y);
		if(u.is_success())
			return u;
		
		if(y->type() == t_unbound) //Special unbound exception
			return y->unify_partial(x);
		
		return u;
	}

	substitution sl_type::unify(const s_ptr<const sl_type> t) const
	{
		unify_details u = unify_internal(t);
		if(!u.is_success())
			throw unification_error(shared_from_this(), t, u.t1(), u.t2());
		
		return u.fetch_s();
	}

	bool sl_type::is_unbound() const
	{
		return false;
	}
	
	void sl_type::print_debug() const
	{
		this->print(std::cout);
		std::cout << std::endl;
	}
}

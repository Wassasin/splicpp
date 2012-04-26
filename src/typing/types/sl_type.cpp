#include "sl_type.hpp"

#include "../../common/generic.hpp"
#include "../typecontext.hpp"
#include "../substitution.hpp"
#include "../unification_error.hpp"

#include "sl_type_unbound.hpp"

namespace splicpp
{
	boost::optional<substitution> sl_type::unify_internal(const cs_ptr<sl_type> t) const
	{
		const cs_ptr<sl_type> x = shared_from_this();
		const cs_ptr<sl_type> y = t;
		
		const boost::optional<substitution> u = x->unify_partial(y);
		if(u)
			return u;
		
		if(y->type() == t_unbound) //Special unbound exception
			return y->unify_partial(x);
		
		return boost::optional<substitution>();
	}

	substitution sl_type::unify(const cs_ptr<sl_type> t) const
	{
		boost::optional<substitution> u = unify_internal(t);
		if(!u)
			throw unification_error(this, t.get());
		
		return u.get();
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

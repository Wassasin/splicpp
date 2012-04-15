#include "sl_type.hpp"

#include "../../common/generic.hpp"
#include "../typecontext.hpp"
#include "../substitution.hpp"
#include "../unification_error.hpp"

#include "sl_type_universal.hpp"
#include "sl_type_unbound.hpp"

namespace splicpp
{
	boost::optional<substitution> sl_type::unify_internal(const std::shared_ptr<sl_type> t) const
	{
		const std::shared_ptr<sl_type> x = apply(substitution::id());
		const std::shared_ptr<sl_type> y = t;
		
		const boost::optional<substitution> u = x->unify_partial(y);
		if(u)
			return u;
		
		if(y->type() == t_unbound) //Special unbound exception
			return y->unify_partial(x);
		
		return boost::optional<substitution>();
	}

	substitution sl_type::unify(const std::shared_ptr<sl_type> t) const
	{
		boost::optional<substitution> u = unify_internal(t);
		if(!u)
			throw unification_error(this, t.get());
		
		return u.get();
	}
	
	std::shared_ptr<sl_type> sl_type::qualify(const typecontext& c) const
	{
		const auto b = subtract_ptr<sl_type_unbound>(tv(), c.fv());
		const auto id = apply(substitution::id()); //Hack to return this
		if(b.size() == 0)
			return id;
		
		std::shared_ptr<sl_type_universal> result(new sl_type_universal(id));
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
	
	void sl_type::print_debug() const
	{
		this->print(std::cout);
		std::cout << std::endl;
	}
}

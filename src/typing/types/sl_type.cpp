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
		std::shared_ptr<sl_type> x = apply(substitution::id());
		std::shared_ptr<sl_type> y = t;
		
		//Assume unbound in case of unification for type declaration; ast_id takes care of proper unbinding in case of function-usage and type inference.
		if(x->type() == t_universal)
			x = std::dynamic_pointer_cast<sl_type_universal>(x)->unbind_naive();
		
		if(y->type() == t_universal)
			y = std::dynamic_pointer_cast<sl_type_universal>(y)->unbind_naive();
		
		if(x->type() != t_unbound && y->type() == t_unbound)
			return y->unify_partial(x);
		
		return x->unify_partial(y);
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

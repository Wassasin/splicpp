#include "sl_polytype.hpp"

#include "sl_type.hpp"
#include "sl_type_unbound.hpp"

#include "../typecontext.hpp"
#include "../../common/generic.hpp"

namespace splicpp
{
	void sl_polytype_forall::bind(std::shared_ptr<sl_type_unbound> t)
	{
		bindings.push_back(t);
	}
	
	std::shared_ptr<sl_type> sl_polytype_forall::unbind(const typecontext& c) const
	{
		substitution s;
		for(const auto binding : bindings)
			s.add(binding, c.create_fresh());
		
		return t->apply(s);
	}
	
	std::shared_ptr<sl_type> sl_polytype_forall::unbind_maintain() const
	{
		return t;
	}
	
	std::vector<std::shared_ptr<sl_type_unbound>> sl_polytype_forall::tv() const
	{
		return subtract_ptr<sl_type_unbound>(t->tv(), bindings);
	}
	
	std::shared_ptr<sl_polytype> sl_polytype_forall::apply(const typecontext& c, const substitution& s) const
	{
		const std::shared_ptr<sl_type> tt = t->apply(s);
		const std::shared_ptr<sl_polytype_forall> result = sl_polytype_forall::not_qualify(tt);
		
		std::vector<std::shared_ptr<sl_type_unbound>> fvs;
		for(const std::shared_ptr<sl_type_unbound> fvtmp : c.fv())
			for(const std::shared_ptr<sl_type_unbound> fv : fvtmp->apply(s)->tv())
				if(!is_in_ptr<sl_type_unbound>(fv, fvs))
					fvs.push_back(fv);
		
		for(const std::shared_ptr<sl_type_unbound> b : bindings)
		{
			const std::shared_ptr<sl_type> bt = b->apply(s);
			if(bt->type() == sl_type::t_unbound)
			{
				const auto btu = std::dynamic_pointer_cast<sl_type_unbound>(bt);
				if(!is_in_ptr<sl_type_unbound>(btu, fvs))
					result->bind(btu);
			}
		}
		
		return std::static_pointer_cast<sl_polytype>(result);
	}
	
	void sl_polytype_forall::print(std::ostream& s) const
	{
		if(bindings.size() > 0)
		{
			s << "(forall";
			for(const auto b : bindings)
				b->print(s << ' ');
		
			t->print(s << " . ");
			s << ')';
		}
		else
			t->print(s);
	}
	
	std::shared_ptr<sl_polytype> sl_polytype::qualify(const typecontext& c, const std::shared_ptr<sl_type> t)
	{
		return std::static_pointer_cast<sl_polytype>(sl_polytype_forall::qualify(c, t));
	}
	
	std::shared_ptr<sl_polytype> sl_polytype::not_qualify(const std::shared_ptr<sl_type> t)
	{
		return std::static_pointer_cast<sl_polytype>(sl_polytype_forall::not_qualify(t));
	}
	
	std::shared_ptr<sl_polytype_forall> sl_polytype_forall::qualify(const typecontext& c, const std::shared_ptr<sl_type> t)
	{
		const auto result = std::shared_ptr<sl_polytype_forall>(new sl_polytype_forall(t));
		
		for(const auto x : subtract_ptr<sl_type_unbound>(t->tv(), c.fv()))
			result->bind(x);
		
		return result;
	}
	
	std::shared_ptr<sl_polytype_forall> sl_polytype_forall::not_qualify(const std::shared_ptr<sl_type> t)
	{
		return std::shared_ptr<sl_polytype_forall>(new sl_polytype_forall(t));
	}
}

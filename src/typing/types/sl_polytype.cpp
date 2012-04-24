#include "sl_polytype.hpp"

#include "sl_type.hpp"
#include "sl_type_unbound.hpp"

#include "../typecontext.hpp"
#include "../../common/generic.hpp"
#include "../../common/utils.hpp"

namespace splicpp
{
	void sl_polytype_forall::bind(cs_ptr<sl_type_unbound> t)
	{
		bindings.push_back(t);
	}
	
	cs_ptr<sl_type> sl_polytype_forall::unbind(const typecontext& c) const
	{
		substitution s;
		for(const auto binding : bindings)
			s.add(binding, c.create_fresh());
		
		return t->apply(s);
	}
	
	cs_ptr<sl_type> sl_polytype_forall::unbind_maintain() const
	{
		return t;
	}
	
	std::vector<cs_ptr<sl_type_unbound>> sl_polytype_forall::tv() const
	{
		return subtract_ptr<const sl_type_unbound>(t->tv(), bindings);
	}
	
	cs_ptr<sl_polytype> sl_polytype_forall::apply(const typecontext& c, const substitution& s) const
	{
		const cs_ptr<sl_type> tt = t->apply(s);
		const s_ptr<sl_polytype_forall> result(new sl_polytype_forall(t));
		
		std::vector<cs_ptr<sl_type_unbound>> fvs;
		for(const cs_ptr<sl_type_unbound> fvtmp : c.fv())
			for(const cs_ptr<sl_type_unbound> fv : fvtmp->apply(s)->tv())
				if(!is_in_ptr<const sl_type_unbound>(fv, fvs))
					fvs.push_back(fv);
		
		for(const cs_ptr<sl_type_unbound> b : bindings)
		{
			const cs_ptr<sl_type> bt = b->apply(s);
			if(bt->type() == sl_type::t_unbound)
			{
				const auto btu = std::dynamic_pointer_cast<const sl_type_unbound>(bt);
				if(!is_in_ptr<const sl_type_unbound>(btu, fvs))
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
	
	cs_ptr<sl_polytype> sl_polytype::qualify(const typecontext& c, const cs_ptr<sl_type> t)
	{
		return std::static_pointer_cast<const sl_polytype>(sl_polytype_forall::qualify(c, t));
	}
	
	cs_ptr<sl_polytype> sl_polytype::not_qualify(const cs_ptr<sl_type> t)
	{
		return std::static_pointer_cast<const sl_polytype>(sl_polytype_forall::not_qualify(t));
	}
	
	cs_ptr<sl_polytype_forall> sl_polytype_forall::qualify(const typecontext& c, const cs_ptr<sl_type> t)
	{
		s_ptr<sl_polytype_forall> result(new sl_polytype_forall(t));
		
		for(const auto x : subtract_ptr<const sl_type_unbound>(t->tv(), c.fv()))
			result->bind(x);
		
		return result;
	}
	
	cs_ptr<sl_polytype_forall> sl_polytype_forall::not_qualify(const cs_ptr<sl_type> t)
	{
		return cs_ptr<sl_polytype_forall>(new sl_polytype_forall(t));
	}
	
	cs_ptr<sl_type> sl_polytype_exists::unbind(const typecontext& c) const
	{
		return std::static_pointer_cast<const sl_type>(c.create_fresh());
	}
		
	std::vector<cs_ptr<sl_type_unbound>> sl_polytype_exists::tv() const
	{
		return std::vector<cs_ptr<sl_type_unbound>>();
	}
	
	cs_ptr<sl_polytype> sl_polytype_exists::apply(const typecontext& c, const substitution&) const
	{
		//return shared_from_this();
		return apply(c, substitution::id()); //To stop errors
	}
	
	void sl_polytype_exists::print(std::ostream& s) const
	{
		s << "(exists T";
		
		if(bindings.size() > 0)
		{
			s << " {bound to: ";
			
			delim_gen_printer<const sl_type_unbound> p(", ", s);
			for(const auto b : bindings)
				p.print(b);
			
			s << "}";
		}
		
		s << ")";
	}
}

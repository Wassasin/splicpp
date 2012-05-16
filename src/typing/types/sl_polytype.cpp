#include "sl_polytype.hpp"

#include "sl_type.hpp"
#include "sl_type_unbound.hpp"

#include "../typecontext.hpp"
#include "../../common/generic.hpp"
#include "../../common/utils.hpp"

namespace splicpp
{
	void sl_polytype_forall::bind(s_ptr<const sl_type_unbound> t)
	{
		bindings.push_back(t);
	}
	
	s_ptr<const sl_type> sl_polytype_forall::unbind(const typecontext& c) const
	{
		substitution s;
		for(const auto binding : bindings)
			s.add(binding, c.create_fresh(t->sl));
		
		return t->apply(s);
	}
	
	s_ptr<const sl_type> sl_polytype_forall::unbind_maintain() const
	{
		return t;
	}
	
	std::vector<s_ptr<const sl_type_unbound>> sl_polytype_forall::tv() const
	{
		return subtract_ptr<const sl_type_unbound>(t->tv(), bindings);
	}
	
	s_ptr<const sl_polytype> sl_polytype_forall::apply(const typecontext& c, const substitution& s) const
	{
		const s_ptr<const sl_type> tt = t->apply(s);
		const s_ptr<sl_polytype_forall> result(new sl_polytype_forall(tt));
		
		std::vector<s_ptr<const sl_type_unbound>> fvs;
		for(const s_ptr<const sl_type_unbound> fvtmp : c.fv())
			for(const s_ptr<const sl_type_unbound> fv : fvtmp->apply(s)->tv())
				if(!is_in_ptr<const sl_type_unbound>(fv, fvs))
					fvs.push_back(fv);
		
		for(const s_ptr<const sl_type_unbound> b : bindings)
			for(const s_ptr<const sl_type_unbound> fv : b->apply(s)->tv())
				if(!is_in_ptr<const sl_type_unbound>(fv, fvs))
					result->bind(fv);
		
		return result;
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
	
	s_ptr<const sl_polytype> sl_polytype::qualify(const typecontext& c, const s_ptr<const sl_type> t)
	{
		return sl_polytype_forall::qualify(c, t);
	}
	
	s_ptr<const sl_polytype> sl_polytype::not_qualify(const s_ptr<const sl_type> t)
	{
		return sl_polytype_forall::not_qualify(t);
	}
	
	s_ptr<const sl_polytype_forall> sl_polytype_forall::qualify(const typecontext& c, const s_ptr<const sl_type> t)
	{
		s_ptr<sl_polytype_forall> result(new sl_polytype_forall(t));
		
		for(const auto x : subtract_ptr<const sl_type_unbound>(t->tv(), c.fv()))
			result->bind(x);
		
		return result;
	}
	
	s_ptr<const sl_polytype_forall> sl_polytype_forall::not_qualify(const s_ptr<const sl_type> t)
	{
		return s_ptr<const sl_polytype_forall>(new sl_polytype_forall(t));
	}
	
	substitution sl_polytype_exists::propagate_findings(const typecontext& c, const s_ptr<const sl_polytype> t, substitution s) const
	{
		for(const auto b : *bindings.get())
		{
			const s_ptr<const sl_type> ct = b->apply(s);
			const s_ptr<const sl_type> tu = t->apply(c, s)->unbind(c); //Unbind for every child separately
			
			s = ct->unify(tu).composite(s);
		}
		
		return s;
	}
	
	std::vector<s_ptr<const sl_type_unbound>> sl_polytype_exists::fetch_bindings() const
	{
		return *bindings.get();
	}
	
	s_ptr<const sl_type> sl_polytype_exists::unbind(const typecontext& c) const
	{
		const auto u = c.create_fresh(sloc()); //Virtual position in code
		bindings->push_back(u);
		return u;
	}
		
	std::vector<s_ptr<const sl_type_unbound>> sl_polytype_exists::tv() const
	{
		return std::vector<s_ptr<const sl_type_unbound>>();
	}
	
	s_ptr<const sl_polytype> sl_polytype_exists::apply(const typecontext&, const substitution&) const
	{
		return shared_from_this();
	}
	
	void sl_polytype_exists::print(std::ostream& s) const
	{
		s << "(exists T";
		
		if(bindings->size() > 0)
		{
			s << " {binds: ";
			
			delim_gen_printer<const sl_type_unbound> p(", ", s);
			for(const auto b : *bindings.get())
				p.print(b);
			
			s << "}";
		}
		
		s << ")";
	}
}

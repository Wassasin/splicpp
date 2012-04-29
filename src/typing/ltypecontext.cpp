#include "ltypecontext.hpp"

#include "types/sl_polytype.hpp"

namespace splicpp
{
	void ltypecontext::register_type(const sid i, const s_ptr<const sl_polytype> t)
	{
		register_global(i, t);
		typecontext::register_type(i, t);
	}

	ltypecontext ltypecontext::apply(const substitution& s) const
	{
		ltypecontext c = *this;
		
		for(size_t i = 0; i < c.types.size(); i++)
			if(c.types[i])
				c.types[i] = c.types[i].get()->apply(*this, s); //Original context
		
		return c;
	}

	void ltypecontext::register_global(const sid i, const s_ptr<const sl_polytype> t) const
	{
		g->register_type(i, t);
	}
}

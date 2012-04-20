#ifndef LTYPECONTEXT_H
#define LTYPECONTEXT_H

#include "typecontext.hpp"

namespace splicpp
{
	class ltypecontext : public typecontext
	{
		std::shared_ptr<typecontext> g;
		
	public:
		ltypecontext(std::shared_ptr<typecontext> g)
		: typecontext(g->ft_count)
		, g(g)
		{}
		
		ltypecontext(ltypecontext& c)
		: typecontext(c.types, c.ft_count)
		, g(c.g)
		{}
		
		ltypecontext(const ltypecontext& c)
		: typecontext(c.types, c.ft_count)
		, g(c.g)
		{}
		
		virtual void register_type(const sid i, const std::shared_ptr<sl_polytype> t);
		
		ltypecontext apply(const substitution& s) const;
		void register_global(const sid i, const std::shared_ptr<sl_polytype> t) const;
	};
}

#endif

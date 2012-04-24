#ifndef SL_TYPE_TUPLE_H
#define SL_TYPE_TUPLE_H

#include "sl_type.hpp"

namespace splicpp
{	
	class sl_type_tuple : public sl_type
	{
		s_ptr<sl_type> t_left, t_right;
		
	protected:
		virtual boost::optional<substitution> unify_partial(const s_ptr<sl_type> t) const;
	
	public:
		sl_type_tuple(const s_ptr<sl_type> t_left, const s_ptr<sl_type> t_right)
		: t_left(t_left)
		, t_right(t_right)
		{}
		
		virtual sl_type_type type() const;
		virtual void print(std::ostream& s) const;
		
		virtual std::vector<s_ptr<sl_type_unbound>> tv() const;
		virtual s_ptr<sl_type> apply(const substitution& s) const;
	};
}

#endif

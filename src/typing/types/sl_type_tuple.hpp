#ifndef SL_TYPE_TUPLE_H
#define SL_TYPE_TUPLE_H

#include "sl_type.hpp"

namespace splicpp
{	
	class sl_type_tuple : public sl_type
	{
		std::shared_ptr<sl_type> t_left, t_right;
	public:
		sl_type_tuple(const std::shared_ptr<sl_type> t_left, const std::shared_ptr<sl_type> t_right)
		: t_left(t_left)
		, t_right(t_right)
		{}
		
		virtual sl_type_type type() const;
		virtual void print(std::ostream& s) const;
		
		virtual std::vector<std::shared_ptr<sl_type_unbound>> tv() const;
		virtual substitution unify(const std::shared_ptr<sl_type> t) const;
		virtual std::shared_ptr<sl_type> apply(const substitution& s) const;
	};
}

#endif

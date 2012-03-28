#ifndef SL_TYPE_ARRAY_H
#define SL_TYPE_ARRAY_H

#include "sl_type.hpp"

namespace splicpp
{	
	class sl_type_array : public sl_type
	{
		std::shared_ptr<sl_type> t;
	public:
		sl_type_array(const std::shared_ptr<sl_type> t)
		: t(t)
		{}
		
		virtual sl_type_type type() const;
		virtual void print(std::ostream& s) const;
		
		virtual std::vector<std::shared_ptr<sl_type_unbound>> tv() const;
		virtual substitution unify(const std::shared_ptr<sl_type> t, typecontext& c) const;
		virtual std::shared_ptr<sl_type> apply(const substitution& s) const;
	};
}

#endif

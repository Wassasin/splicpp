#ifndef SL_TYPE_BOOL_H
#define SL_TYPE_BOOL_H

#include "sl_type.hpp"

namespace splicpp
{	
	class sl_type_bool : public sl_type
	{
	protected:
		virtual substitution unify_partial(const std::shared_ptr<sl_type> t) const;
	
	public:
		virtual sl_type_type type() const;
		virtual void print(std::ostream& s) const;
		
		virtual std::vector<std::shared_ptr<sl_type_unbound>> tv() const;
		virtual std::shared_ptr<sl_type> apply(const substitution& s) const;
	};
}

#endif

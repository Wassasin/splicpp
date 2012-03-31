#ifndef SL_TYPE_VOID_H
#define SL_TYPE_VOID_H

#include "sl_type.hpp"

namespace splicpp
{	
	class sl_type_void : public sl_type
	{
	public:
		virtual sl_type_type type() const;
		virtual void print(std::ostream& s) const;
		
		virtual std::vector<std::shared_ptr<sl_type_unbound>> tv() const;
		virtual substitution unify(const std::shared_ptr<sl_type> t) const;
		virtual std::shared_ptr<sl_type> apply(const substitution& s) const;
	};
}

#endif

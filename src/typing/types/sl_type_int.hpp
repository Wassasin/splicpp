#ifndef SL_TYPE_INT_H
#define SL_TYPE_INT_H

#include "sl_type.hpp"

namespace splicpp
{	
	class sl_type_int : public sl_type
	{
	protected:
		virtual unify_details unify_partial(const s_ptr<const sl_type> t) const;
	
	public:
		sl_type_int(const sloc sl)
		: sl_type(sl)
		{}
	
		virtual sl_type_type type() const;
		virtual void print(std::ostream& s) const;
		
		virtual std::vector<s_ptr<const sl_type_unbound>> tv() const;
		virtual s_ptr<const sl_type> apply(const substitution& s) const;
	};
}

#endif

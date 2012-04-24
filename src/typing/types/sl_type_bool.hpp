#ifndef SL_TYPE_BOOL_H
#define SL_TYPE_BOOL_H

#include "sl_type.hpp"

namespace splicpp
{	
	class sl_type_bool : public sl_type
	{
	protected:
		virtual boost::optional<substitution> unify_partial(const s_ptr<sl_type> t) const;
	
	public:
		virtual sl_type_type type() const;
		virtual void print(std::ostream& s) const;
		
		virtual std::vector<s_ptr<sl_type_unbound>> tv() const;
		virtual s_ptr<sl_type> apply(const substitution& s) const;
	};
}

#endif

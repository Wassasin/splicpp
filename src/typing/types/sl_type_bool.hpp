#ifndef SL_TYPE_BOOL_H
#define SL_TYPE_BOOL_H

#include "sl_type.hpp"

namespace splicpp
{	
	class sl_type_bool : public sl_type
	{
	protected:
		virtual boost::optional<substitution> unify_partial(const cs_ptr<sl_type> t) const;
	
	public:
		sl_type_bool(const sloc sl)
		: sl_type(sl)
		{}
	
		virtual sl_type_type type() const;
		virtual void print(std::ostream& s) const;
		
		virtual std::vector<cs_ptr<sl_type_unbound>> tv() const;
		virtual cs_ptr<sl_type> apply(const substitution& s) const;
	};
}

#endif

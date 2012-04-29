#ifndef SL_TYPE_FUNCTION_H
#define SL_TYPE_FUNCTION_H

#include <vector>
#include <memory>

#include "sl_type.hpp"

namespace splicpp
{	
	class sl_type_function : public sl_type
	{
		std::vector<s_ptr<const sl_type>> args;
		s_ptr<const sl_type> r;
		
	protected:
		virtual boost::optional<substitution> unify_partial(const s_ptr<const sl_type> t) const;
	
	public:
		sl_type_function(const std::vector<s_ptr<const sl_type>> args, const s_ptr<const sl_type> r, const sloc sl)
		: sl_type(sl)
		, args(args)
		, r(r)
		{}
		
		virtual sl_type_type type() const;
		virtual void print(std::ostream& s) const;
		
		virtual std::vector<s_ptr<const sl_type_unbound>> tv() const;
		virtual s_ptr<const sl_type> apply(const substitution& s) const;
	};
}

#endif

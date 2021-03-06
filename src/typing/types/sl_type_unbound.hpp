#ifndef SL_TYPE_UNBOUND_H
#define SL_TYPE_UNBOUND_H

#include "sl_type.hpp"

namespace splicpp
{
	class sl_type;
	class substitution;

	class sl_type_unbound : public sl_type
	{
		const uint id;
		
	protected:
		virtual unify_details unify_partial(const s_ptr<const sl_type> t) const;
	
	public:
		sl_type_unbound(const uint id, const sloc sl)
		: sl_type(sl)
		, id(id)
		{}
		
		virtual bool is_unbound() const;
		
		virtual sl_type_type type() const;
		bool equals(const s_ptr<const sl_type_unbound> y) const;
		bool operator<(const s_ptr<const sl_type_unbound> y) const;
		bool operator==(const s_ptr<const sl_type_unbound> y) const;
		virtual void print(std::ostream& s) const;
		
		virtual std::vector<s_ptr<const sl_type_unbound>> tv() const;
		virtual s_ptr<const sl_type> apply(const substitution& s) const;
	};
}

#endif

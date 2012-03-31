#ifndef SL_TYPE_UNIVERSAL_H
#define SL_TYPE_UNIVERSAL_H

#include "sl_type.hpp"

namespace splicpp
{
	class sl_type;
	class sl_type_unbound;
	class substitution;

	class sl_type_universal : public sl_type
	{
		std::shared_ptr<sl_type> t;
		std::vector<std::shared_ptr<sl_type_unbound>> bindings;
	public:
		sl_type_universal(const std::shared_ptr<sl_type> t)
		: t(t)
		, bindings()
		{}
		
		void bind(std::shared_ptr<sl_type_unbound> t);
		std::shared_ptr<sl_type> unbind(const typecontext& c) const;
		
		virtual sl_type_type type() const;
		virtual void print(std::ostream& s) const;
		
		virtual std::vector<std::shared_ptr<sl_type_unbound>> tv() const;
		virtual substitution unify(const std::shared_ptr<sl_type> t) const;
		virtual std::shared_ptr<sl_type> apply(const substitution& s) const;
	};
}

#endif

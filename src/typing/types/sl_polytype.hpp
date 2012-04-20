#ifndef SL_POLYTYPE_H
#define SL_POLYTYPE_H

#include "sl_type.hpp"

namespace splicpp
{
	class sl_type;
	class sl_type_unbound;
	class substitution;

	class sl_polytype
	{
		std::shared_ptr<sl_type> t;
		std::vector<std::shared_ptr<sl_type_unbound>> bindings;
		
	public:
		sl_polytype(const std::shared_ptr<sl_type> t)
		: t(t)
		, bindings()
		{}
		
		void bind(std::shared_ptr<sl_type_unbound> t);
		
		std::shared_ptr<sl_type> unbind(const typecontext& c) const;
		std::shared_ptr<sl_type> unbind_maintain() const;
		
		std::vector<std::shared_ptr<sl_type_unbound>> tv() const;
		std::shared_ptr<sl_polytype> apply(const typecontext& c, const substitution& s) const;
		
		void print(std::ostream& s) const;
		
		static std::shared_ptr<sl_polytype> qualify(const typecontext& c, const std::shared_ptr<sl_type> t);
		static std::shared_ptr<sl_polytype> not_qualify(const std::shared_ptr<sl_type> t);
	};
}

#endif

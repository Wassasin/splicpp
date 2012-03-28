#ifndef SL_TYPE_H
#define SL_TYPE_H

#include <vector>
#include <memory>
#include <ostream>

#include "../substitution.hpp"

namespace splicpp
{
	class substitution;
	class typecontext;
	class sl_type_unbound;

	class sl_type
	{
	public:
		enum sl_type_type
		{
			t_function,
			t_tuple,
			t_array,
			t_unbound,
			t_int,
			t_bool
		};
		
		virtual ~sl_type() {}
		virtual bool is_unbound() const;
		
		virtual sl_type_type type() const = 0;
		virtual void print(std::ostream& s) const = 0;
		
		virtual std::vector<std::shared_ptr<sl_type_unbound>> tv() const = 0;
		virtual substitution unify(const std::shared_ptr<sl_type> t, typecontext& c) const = 0;
		virtual std::shared_ptr<sl_type> apply(const substitution& s) const = 0;
	};
}

#endif

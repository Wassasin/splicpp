#ifndef SL_TYPE_H
#define SL_TYPE_H

#include <vector>
#include <memory>
#include <ostream>
#include <boost/optional.hpp>

#include "../../common/sloc.hpp"
#include "../substitution.hpp"

namespace splicpp
{
	class substitution;
	class typecontext;
	class sl_type_unbound;

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Weffc++"
	/*
		Ignore non-virtual-destructor warning
		This is a bug in GCC4.6
		See http://stackoverflow.com/questions/2571850/why-does-enable-shared-from-this-have-a-non-virtual-destructor
	*/
	class sl_type : public std::enable_shared_from_this<sl_type>
	{
	protected:
		virtual boost::optional<substitution> unify_partial(const s_ptr<const sl_type> t) const = 0;
		
	public:
		enum sl_type_type
		{
			t_function,
			t_tuple,
			t_array,
			t_unbound,
			t_int,
			t_bool,
			t_void,
			t_universal
		};
		
		const sloc sl;
		
		sl_type(const sloc sl)
		: sl(sl)
		{}
		
		substitution unify(const s_ptr<const sl_type> t) const;
		boost::optional<substitution> unify_internal(const s_ptr<const sl_type> t) const;
		
		void print_debug() const;
		
		virtual ~sl_type() {}
		virtual bool is_unbound() const;
		
		virtual sl_type_type type() const = 0;
		virtual void print(std::ostream& s) const = 0;
		
		virtual std::vector<s_ptr<const sl_type_unbound>> tv() const = 0;
		virtual s_ptr<const sl_type> apply(const substitution& s) const = 0;
	};
	#pragma GCC diagnostic pop
}

#endif

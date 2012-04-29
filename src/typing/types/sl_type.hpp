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
		struct unify_details
		{
			boost::optional<substitution> s;
			boost::optional<std::tuple<s_ptr<const sl_type>, s_ptr<const sl_type>>> ts;
			
			unify_details(const substitution s) //Success
			: s(s)
			, ts()
			{}
			
			unify_details(const s_ptr<const sl_type> t1, const s_ptr<const sl_type> t2) //Failure
			: s()
			, ts(std::make_tuple(t1, t2))
			{}
			
			bool is_success() const { return s; }
			
			substitution fetch_s() const { return s.get(); }
			s_ptr<const sl_type> t1() const { return std::get<0>(ts.get()); }
			s_ptr<const sl_type> t2() const { return std::get<1>(ts.get()); }
		};
		
		virtual unify_details unify_partial(const s_ptr<const sl_type> t) const = 0;
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
		unify_details unify_internal(const s_ptr<const sl_type> t) const;
		
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

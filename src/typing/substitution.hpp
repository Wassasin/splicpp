#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include <map>
#include <ostream>

#include "../common/typedefs.hpp"

namespace splicpp
{
	class sl_type;
	class sl_type_unbound;

	class substitution
	{
		template<class T> 
		struct pointer_less
		{
			bool operator()( const s_ptr<const T>& lhs, const s_ptr<const T>& rhs ) const
			{
				return lhs->operator<(rhs);
			}
		};
	
		std::map<s_ptr<const sl_type_unbound>, s_ptr<const sl_type>, pointer_less<sl_type_unbound>> subs;
		
	public:
		substitution()
		: subs()
		{}
		
		void add(const s_ptr<const sl_type_unbound> x, s_ptr<const sl_type> y);
		void set(const s_ptr<const sl_type_unbound> x, const s_ptr<const sl_type> y);
		
		s_ptr<const sl_type> substitute(const s_ptr<const sl_type_unbound> x) const;
		substitution composite(const substitution& s) const;
		void print(std::ostream& s) const;
		
		static substitution id()
		{
			return substitution();
		}
	};
}

#endif

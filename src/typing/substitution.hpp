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
			bool operator()( const cs_ptr<T>& lhs, const cs_ptr<T>& rhs ) const
			{
				return lhs->operator<(rhs);
			}
		};
	
		std::map<cs_ptr<sl_type_unbound>, cs_ptr<sl_type>, pointer_less<sl_type_unbound>> subs;
		
	public:
		substitution()
		: subs()
		{}
		
		void add(const cs_ptr<sl_type_unbound> x, cs_ptr<sl_type> y);
		void set(const cs_ptr<sl_type_unbound> x, const cs_ptr<sl_type> y);
		
		cs_ptr<sl_type> substitute(const cs_ptr<sl_type_unbound> x) const;
		substitution composite(const substitution& s) const;
		void print(std::ostream& s) const;
		
		static substitution id()
		{
			return substitution();
		}
	};
}

#endif

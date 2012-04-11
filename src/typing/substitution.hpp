#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include <map>
#include <memory>
#include <ostream>

namespace splicpp
{
	class sl_type;
	class sl_type_unbound;

	class substitution
	{
		template<class T> 
		struct pointer_less
		{
			bool operator()( const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs ) const
			{
				return lhs->operator<(rhs);
			}
		};
	
		std::map<std::shared_ptr<sl_type_unbound>, std::shared_ptr<sl_type>, pointer_less<sl_type_unbound>> subs;
		
	public:
		substitution()
		: subs()
		{}
		
		void add(const std::shared_ptr<sl_type_unbound> x, std::shared_ptr<sl_type> y);
		void set(const std::shared_ptr<sl_type_unbound> x, const std::shared_ptr<sl_type> y);
		
		std::shared_ptr<sl_type> substitute(const std::shared_ptr<sl_type_unbound> x) const;
		substitution composite(const substitution& s) const;
		void print(std::ostream& s) const;
		
		static substitution id()
		{
			return substitution();
		}
	};
}

#endif

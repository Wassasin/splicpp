#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

#include <vector>
#include <memory>
#include <ostream>

namespace splicpp
{
	class sl_type;
	class sl_type_unbound;

	class substitution
	{
		std::vector<std::pair<std::shared_ptr<sl_type_unbound>, std::shared_ptr<sl_type>>> subs;
		
	public:
		substitution()
		: subs()
		{}
		
		void add(const std::shared_ptr<sl_type_unbound> x, const std::shared_ptr<sl_type> y);
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

#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <boost/array.hpp>

#include "../common/rule.h"
#include "../common/typedefs.h"

namespace splicpp
{
	template <size_t L>
	struct item
	{
		rid rule;
		size_t dot;
		boost::array<stid, L> lookahead;

		item(rid rule, uint dot)
		: rule(rule)
		, dot(dot)
		, lookahead()
		{}

		item(rid rule, uint dot, decltype(lookahead) lookahead)
		: rule(rule)
		, dot(dot)
		, lookahead(lookahead)
		{}
		
		bool operator==(const item<L>& y) const
		{
			if(this->rule != y.rule || this->dot != y.dot)
				return false;

			size_t i_size = L;
			for(size_t i = 0; i < i_size; i++)
				if(this->lookahead[i] != y.lookahead[i])
					return false;
			
			return true;
		}

		bool at_end(const grammar g) const
		{
			return(g.fetch_rule(rule).body.size() == dot);
		}

		stid after_dot(const grammar g) const
		{
			return g.fetch_rule(rule).body[dot];
		}

		stid before_dot(const grammar g) const
		{
			return g.fetch_rule(rule).body[dot-1];
		}

		item<L> next() const
		{
			if(at_end())
				throw std::exception(); //TODO

			return item(rule, dot+1, lookahead);
		}

		bool is_in(std::vector<item<L>> i_set) const
		{
			for(size_t i = 0; i < i_set.size(); i++)
				if((*this) == i_set[i])
					return true;

			return false;
		}
	};
}

#endif



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
		
		void print(const grammar g) const
		{
			const splicpp::rule r = g.fetch_rule(rule);
		
			std::cout << g.fetch_symbol(r.start)->name << " -> ";
			
			for(size_t i = 0; i < r.body.size(); i++)
			{
				if(dot == i)
					std::cout << '.';
				
				std::cout << g.fetch_symbol(r.body[i]) << ' ';
			}
			
			if(dot == r.body.size())
				std::cout << ". ";
			
			size_t i_size = L;
			if(i_size > 0)
			{
				std::cout << '[';
				for(size_t i = 0; i < i_size; i++)
				{
					if(i != 0)
						std::cout << ' ';
				
					std::cout << g.fetch_symbol(lookahead[i])->name;
				}
				std::cout << ']';
			}
		}
	};
}

#endif



#ifndef ITEM_H
#define ITEM_H

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
		: dot(dot)
		: lookahead()
		{}
		
		bool operator==(const item<L> y)
		{
			if(this->rule != y.rule || this->dot != y.dot)
				return false;
			
			for(size_t i = 0; i < L; i++)
				if(this->lookahead[i] != y.lookahead[i])
					return false;
			
			return true;
		}
	};
}

#endif



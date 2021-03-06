#ifndef RULE_H
#define RULE_H

#include <vector>

#include "typedefs.hpp"

namespace splicpp
{
	struct rule
	{
		stid start;
		std::vector<stid> body;
		
		rule(stid start)
		: start(start)
		, body()
		{}
		
		void append_body(const stid s);
		rule operator+(stid s);
		bool operator==(const rule x) const;
		bool operator!=(const rule x) const;
		bool is_epsilon() const;
		void print() const;
	};
}

#endif



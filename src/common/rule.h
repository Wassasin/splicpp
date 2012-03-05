#ifndef RULE_H
#define RULE_H

#include <vector>

#include "typedefs.h"

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
		bool is_epsilon() const;
		void print() const;
	};
}

#endif



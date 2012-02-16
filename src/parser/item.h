#ifndef ITEM_H
#define ITEM_H

#include "../common/rule.h"
#include "../common/typedefs.h"

namespace splicpp
{
	struct item
	{
		rid rule;
		uint dot;
		stid lookahead;
	};
}

#endif



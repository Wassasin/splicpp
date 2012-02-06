#ifndef TOKEN_H
#define TOKEN_H

#include "typedefs.h"

namespace splicpp
{
	struct token
	{
		uint type;
		uint sourcePos;
		uint sourceLength;
	};
}

#endif

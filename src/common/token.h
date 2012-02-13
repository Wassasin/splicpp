#ifndef TOKEN_H
#define TOKEN_H

#include "typedefs.h"

namespace splicpp
{
	struct token
	{
		stid type;
		uint pos, length, line;
		
		token(stid type, uint pos, uint length, uint line)
		: type(type)
		, pos(pos)
		, length(length)
		, line(line)
		{}
	};
}

#endif

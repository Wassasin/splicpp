#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#include "typedefs.hpp"
#include "sloc.hpp"

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
				
		std::string as_string(const std::string source) const;
		
		sloc as_sloc() const
		{
			return sloc(pos, line);
		}
	};
}

#endif

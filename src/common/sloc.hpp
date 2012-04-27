#ifndef SLOC_H
#define SLOC_H

#include <ostream>

namespace splicpp
{
	struct sloc //Source Location
	{
		uint pos, line, length;
		bool fake;
		
		sloc()
		: pos(0)
		, line(0)
		, length(0)
		, fake(true)
		{}
		
		sloc(uint pos, uint line, uint length)
		: pos(pos)
		, line(line)
		, length(length)
		, fake(false)
		{}
		
		void print(std::string str, std::ostream& s) const;
	};
}

#endif

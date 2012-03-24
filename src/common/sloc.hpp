#ifndef SLOC_H
#define SLOC_H

namespace splicpp
{
	struct sloc //Source Location
	{
		uint pos, line;
		
		sloc(uint pos, uint line)
		: pos(pos)
		, line(line)
		{}
	};
}

#endif

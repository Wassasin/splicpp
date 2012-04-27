#ifndef SLOC_H
#define SLOC_H

namespace splicpp
{
	struct sloc //Source Location
	{
		uint pos, line;
		bool fake;
		
		sloc()
		: pos(0)
		, line(0)
		, fake(true)
		{}
		
		sloc(uint pos, uint line)
		: pos(pos)
		, line(line)
		, fake(false)
		{}
	};
}

#endif

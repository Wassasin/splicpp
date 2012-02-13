#ifndef PTABLE_H
#define PTABLE_H

#include <vector>
#include <boost/array.hpp>

#include "typedefs.h"

namespace splicpp
{
	class ptable
	{
		enum transtype
		{
			error,
			accept,
			shift,
			reduce
		};

		struct transition
		{
			transtype t;
			uint state;
		};

		uint terminals, nonterminals;

		std::vector<std::vector<transition>> acttable;
		std::vector<uint> gototable;

	public:
		ptable(uint terminals, uint nonterminals)
		: terminals(terminals)
		, nonterminals(nonterminals)
		, acttable()
		, gototable()
		{}
	};
}

#endif

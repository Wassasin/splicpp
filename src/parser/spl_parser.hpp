#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include "spl.hpp"
#include "ptable.hpp"
#include "clr_parser_gen.hpp"

namespace splicpp
{
	class spl_parser
	{
		const spl g;
		const ptable t;
		
	public:
		spl_parser()
		: g()
		, t(splicpp::clr_parser_gen::generate(g))
		{
			t.print(g);
		}
	};
}

#endif

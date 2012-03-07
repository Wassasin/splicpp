#ifndef SPL_H
#define SPL_H

#include <boost/algorithm/string/trim.hpp>

#include "bnf_parser.hpp"
#include "../common/io.hpp"

namespace splicpp
{
	class spl : public grammar
	{
	public:
		const stid NL_PROG;
	
		spl()
		: grammar()

		, NL_PROG(add_symbol(		new non_literal("nl_prog")))
		{
			ignore(' ');
			ignore('\t');
			
			splicpp::bnf_parser p;
			std::string lang = readfile("grammars/spl.bnf");
			boost::algorithm::trim(lang);
			
			p.parse((*this), lang);
			
			add_rule(rule(NL_START) + NL_PROG);
		}
	};
}

#endif

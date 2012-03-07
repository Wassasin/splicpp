#ifndef BNF_PARSER_H
#define BNF_PARSER_H

#include "bnf.hpp"
#include "ptable.hpp"
#include "clr_parser_gen.hpp"

namespace splicpp
{
	class bnf_parser
	{
		const bnf g_bnf;
		const ptable t;
	
	public:
		bnf_parser()
		: g_bnf()
		, t(splicpp::clr_parser_gen::generate(g_bnf))
		{
			t.print(g_bnf);
		}
		
		void parse(grammar& g, const std::string lang);
	};
}

#endif

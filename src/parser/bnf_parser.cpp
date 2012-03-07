#include "bnf_parser.hpp"

namespace splicpp
{
		void bnf_parser::parse(grammar& g, const std::string lang)
		{
			splicpp::lexer l(g_bnf, lang);
			cst_element e = t.parse(l);
			
//			parse_syntax(g, lang, e.as_node()[0]->as_node());
		}
}

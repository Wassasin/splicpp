#include "spl_parser.hpp"

#include "lexer.hpp"

namespace splicpp
{
		void spl_parser::parse(std::string str) const
		{
			lexer l(g, str);
			cst_element e = t.parse(l);
			
			parse_prog(str, e.as_node()[0]->as_node());
		}
		
		void spl_parser::print_g() const
		{
			g.print();
		}
		
		void spl_parser::print_t() const
		{
			t.print(g);
		}
}

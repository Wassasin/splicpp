#ifndef AST_H
#define AST_H

#include <ostream>

namespace splicpp
{
	class ast
	{
	public:
		virtual void pretty_print(std::ostream& s, const uint tab) const = 0;
		
		virtual ~ast() {}
		
		static void print_newline(std::ostream& s, const uint tab)
		{
			s << std::endl;
			for(uint i = 0; i < tab; i++)
				s << "\t";
		}
	};
}

#endif

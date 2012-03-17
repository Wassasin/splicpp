#ifndef AST_H
#define AST_H

#include <ostream>

namespace splicpp
{
	class ast
	{
	public:
		virtual void pretty_print(std::ostream& s) const = 0;
		
		virtual ~ast() {}
	};
}

#endif

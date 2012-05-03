#ifndef IR_STMT_H
#define IR_STMT_H

#include <ostream>

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_stmt
	{
	public:
		ir_stmt()
		{}
	
		virtual void print(std::ostream& s, const uint tab) const = 0;
		
		virtual ~ir_stmt() {}
		
		static void cat(s_ptr<const ir_stmt>& r, const s_ptr<const ir_stmt> x);
		
		static void print_newline(std::ostream& s, const uint tab)
		{
			s << std::endl;
			for(uint i = 0; i < tab; i++)
				s << "\t";
		}
	};
}

#endif

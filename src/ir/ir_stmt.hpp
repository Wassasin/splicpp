#ifndef IR_STMT_H
#define IR_STMT_H

#include <ostream>

namespace splicpp
{
	class ir_stmt
	{
	public:
		ir_stmt()
		{}
	
		virtual void print(std::ostream& s, const uint tab) const = 0;
		
		virtual ~ir_stmt() {}
		
		static void print_newline(std::ostream& s, const uint tab)
		{
			s << std::endl;
			for(uint i = 0; i < tab; i++)
				s << "\t";
		}
	};
}

#endif

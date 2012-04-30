#ifndef IR_EXP_H
#define IR_EXP_H

#include <ostream>

namespace splicpp
{
	class ir_exp
	{
	public:
		ir_exp()
		{}
	
		virtual void print(std::ostream& s, const uint tab) const = 0;
		
		virtual ~ir_exp() {}
		
		static void print_newline(std::ostream& s, const uint tab)
		{
			s << std::endl;
			for(uint i = 0; i < tab; i++)
				s << "\t";
		}
	};
}

#endif

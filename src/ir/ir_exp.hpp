#ifndef IR_EXP_H
#define IR_EXP_H

#include <ostream>
#include <memory>

namespace splicpp
{
	class ir_exp_transformer;
	
	class ir_exp : public std::enable_shared_from_this<ir_exp>
	{
	public:
		ir_exp()
		{}
		
		virtual ~ir_exp() {}
		
		virtual void transform(ir_exp_transformer& t) const = 0;
		virtual void print(std::ostream& s, const uint tab) const = 0;
		
		static void print_newline(std::ostream& s, const uint tab)
		{
			s << std::endl;
			for(uint i = 0; i < tab; i++)
				s << "\t";
		}
	};
}

#endif

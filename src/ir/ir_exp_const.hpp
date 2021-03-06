#ifndef IR_EXP_CONST_H
#define IR_EXP_CONST_H

#include "ir_exp.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp_const : public ir_exp
	{
	public:
		const int i;
	
		ir_exp_const(const bool b)
		: i((int)b)
		{}
	
		ir_exp_const(const int i)
		: i(i)
		{}
	
		virtual void map(ir_exp_mapper& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
	};
}

#endif

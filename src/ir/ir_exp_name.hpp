#ifndef IR_EXP_NAME_H
#define IR_EXP_NAME_H

#include "ir_exp.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp_name : public ir_exp
	{
	public:
		const ir_label l;
	
		ir_exp_name(const ir_label l)
		: l(l)
		{}
	
		virtual void map(ir_exp_mapper& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
	};
}

#endif

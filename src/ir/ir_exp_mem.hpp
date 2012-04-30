#ifndef IR_EXP_MEM_H
#define IR_EXP_MEM_H

#include "ir_exp.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp_mem : public ir_exp
	{
		const s_ptr<const ir_exp> e;
		
	public:
		ir_exp_mem(const s_ptr<const ir_exp> e)
		: e(e)
		{}
	
		virtual void print(std::ostream& s, const uint tab) const;
	};
}

#endif

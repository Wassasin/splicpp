#ifndef IR_EXP_MEM_H
#define IR_EXP_MEM_H

#include "ir_exp.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp_mem : public ir_exp
	{
	public:
		const s_ptr<const ir_exp> e;
	
		ir_exp_mem(const s_ptr<const ir_exp> e)
		: e(e)
		{}
	
		virtual void map(ir_exp_mapper& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_exp> create(const s_ptr<const ir_exp> e)
		{
			return make_s<ir_exp_mem>(e);
		}
	};
}

#endif

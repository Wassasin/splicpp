#ifndef IR_EXP_TEMP_H
#define IR_EXP_TEMP_H

#include "ir_exp.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp_temp : public ir_exp
	{
		const ir_temp t;
		
	public:
		ir_exp_temp(const ir_temp t)
		: t(t)
		{}
	
		virtual void transform(ir_exp_transformer& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_exp> create(const ir_temp t)
		{
			return s_ptr<const ir_exp>(new ir_exp_temp(t));
		}
	};
}

#endif

#ifndef IR_EXP_NAME_H
#define IR_EXP_NAME_H

#include "ir_exp.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp_name : public ir_exp
	{
		const ir_label l;
		
	public:
		ir_exp_name(const ir_label l)
		: l(l)
		{}
	
		virtual void transform(ir_exp_transformer& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_exp> create(const ir_label l)
		{
			return s_ptr<const ir_exp>(new ir_exp_name(l));
		}
	};
}

#endif

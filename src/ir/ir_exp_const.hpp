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
	
		ir_exp_const(const int i)
		: i(i)
		{}
	
		virtual void transform(ir_exp_transformer& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_exp> create(const int i)
		{
			return s_ptr<const ir_exp>(new ir_exp_const(i));
		}
		
		static s_ptr<const ir_exp> create(const bool b)
		{
			if(b)
				return create(1);
			else
				return create(0);
		}
	};
}

#endif

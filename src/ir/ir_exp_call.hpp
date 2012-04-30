#ifndef IR_EXP_CALL_H
#define IR_EXP_CALL_H

#include "ir_exp.hpp"

#include <vector>
#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_stmt;

	class ir_exp_call : public ir_exp
	{
		const s_ptr<const ir_exp> func;
		const std::vector<s_ptr<const ir_exp>> args;
		
	public:
		ir_exp_call(const s_ptr<const ir_exp> func, const std::vector<s_ptr<const ir_exp>> args)
		: func(func)
		, args(args)
		{}
	
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_exp> create(const s_ptr<const ir_exp> func, const std::vector<s_ptr<const ir_exp>> args)
		{
			return s_ptr<const ir_exp>(new ir_exp_call(func, args));
		}
	};
}

#endif

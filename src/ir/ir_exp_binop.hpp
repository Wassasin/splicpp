#ifndef IR_EXP_BINOP_H
#define IR_EXP_BINOP_H

#include "ir_exp.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp_binop : public ir_exp
	{
	public:
		enum binop {
			op_plus,
			op_minus,
			op_mul,
			op_div,
			op_mod,
			op_and,
			op_or,
			op_lshift,
			op_rshift,
			op_arshift,
			op_xor
		};
	
	private:
		static std::string binop_to_string(const binop op);
		
	public:
		const binop op;
		const s_ptr<const ir_exp> e_left, e_right;
	
		ir_exp_binop(const binop op, const s_ptr<const ir_exp> e_left, const s_ptr<const ir_exp> e_right)
		: op(op)
		, e_left(e_left)
		, e_right(e_right)
		{}
	
		virtual void transform(ir_exp_transformer& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_exp> create(const binop op, const s_ptr<const ir_exp> e_left, const s_ptr<const ir_exp> e_right)
		{
			return s_ptr<const ir_exp>(new ir_exp_binop(op, e_left, e_right));
		}
	};
}

#endif

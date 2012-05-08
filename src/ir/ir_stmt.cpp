#include "ir_stmt.hpp"

#include "ircontext.hpp"

#include "ir_exp_binop.hpp"
#include "ir_exp_const.hpp"
#include "ir_exp_mem.hpp"
#include "ir_exp_temp.hpp"
#include "ir_stmt_move.hpp"
#include "ir_stmt_seq.hpp"

namespace splicpp
{
	void ir_stmt::cat(s_ptr<const ir_stmt>& r, const s_ptr<const ir_stmt> x)
	{
		r = ir_stmt_seq::create(r, x);
	}
	
	s_ptr<const ir_stmt> ir_stmt::push(const std::vector<s_ptr<const ir_exp>>& xs, const ircontext& c)
	{
		const s_ptr<const ir_exp> temp = ir_exp_temp::create(c.create_temporary());
		const s_ptr<const ir_exp> stack = ir_exp_temp::create(c.stack_reg);
	
		s_ptr<const ir_stmt> r(ir_stmt_move::create(
			temp,
			stack
		));
		
		ir_stmt::cat(r, ir_stmt_move::create(
			stack,
			ir_exp_binop::create(
				ir_exp_binop::op_plus,
				stack,
				ir_exp_const::create((int)xs.size())
			)
		));
		
		for(size_t i = 0; i < xs.size(); i++)
			ir_stmt::cat(r, ir_stmt_move::create(
				ir_exp_mem::create(ir_exp_binop::create(
						ir_exp_binop::op_plus,
						temp,
						ir_exp_const::create((int)i)
				)),
				xs[i]
			));
		
		return r;
	}
	
	s_ptr<const ir_stmt> ir_stmt::pop(const size_t count, const ircontext& c)
	{
		const s_ptr<const ir_exp> stack = ir_exp_temp::create(c.stack_reg);
		
		return ir_stmt_move::create(
			stack,
			ir_exp_binop::create(
				ir_exp_binop::op_minus,
				stack,
				ir_exp_const::create((int)count)
			)
		);
	}
}

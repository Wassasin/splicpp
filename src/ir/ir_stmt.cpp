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
		r = make_s<ir_stmt_seq>(r, x);
	}
	
	s_ptr<const ir_stmt> ir_stmt::push(const std::vector<s_ptr<const ir_exp>>& xs, const ircontext& c)
	{
		const s_ptr<const ir_exp> temp = make_s<ir_exp_temp>(c.create_temporary());
		const s_ptr<const ir_exp> stack = make_s<ir_exp_temp>(c.stack_reg);
	
		s_ptr<const ir_stmt> r(make_s<ir_stmt_move>(
			temp,
			stack
		));
		
		ir_stmt::cat(r, make_s<ir_stmt_move>(
			stack,
			make_s<ir_exp_binop>(
				ir_exp_binop::op_minus,
				stack,
				make_s<ir_exp_const>((int)xs.size())
			)
		));
		
		for(size_t i = 0; i < xs.size(); i++)
			ir_stmt::cat(r, make_s<ir_stmt_move>(
				make_s<ir_exp_mem>(make_s<ir_exp_binop>(
						ir_exp_binop::op_minus,
						temp,
						make_s<ir_exp_const>((int)i)
				)),
				xs[i]
			));
		
		return r;
	}
	
	s_ptr<const ir_stmt> ir_stmt::pop(const size_t count, const ircontext& c)
	{
		const s_ptr<const ir_exp> stack = make_s<ir_exp_temp>(c.stack_reg);
		
		return make_s<ir_stmt_move>(
			stack,
			make_s<ir_exp_binop>(
				ir_exp_binop::op_plus,
				stack,
				make_s<ir_exp_const>((int)count)
			)
		);
	}
}

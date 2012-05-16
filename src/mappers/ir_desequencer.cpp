#include "ir_desequencer.hpp"

#include "../ir/ir_exp_binop.hpp"
#include "../ir/ir_exp_const.hpp"
#include "../ir/ir_exp_eseq.hpp"
#include "../ir/ir_exp_mem.hpp"
#include "../ir/ir_exp_name.hpp"
#include "../ir/ir_exp_temp.hpp"

#include "../ir/ir_stmt_call.hpp"
#include "../ir/ir_stmt_cjump.hpp"
#include "../ir/ir_stmt_jump.hpp"
#include "../ir/ir_stmt_label.hpp"
#include "../ir/ir_stmt_move.hpp"
#include "../ir/ir_stmt_seq.hpp"

namespace splicpp
{
	void ir_desequencer::produce(s_ptr<const ir_exp> r)
	{
		acc = r;
	}
	
	s_ptr<const ir_exp> ir_desequencer::map(const s_ptr<const ir_exp> x)
	{
		x->map(*this);
		return acc;
	}
	
	void ir_desequencer::map(const s_ptr<const ir_stmt> x)
	{
		x->map(*this);
	}
	
	std::vector<s_ptr<const ir_stmt>> ir_desequencer::desequence(const s_ptr<const ir_stmt> x)
	{
		ir_desequencer d;
		d.map(x);
		return d.stmts;
	}
	
	//Inherited from ir_exp_transformer
	void ir_desequencer::map(const s_ptr<const ir_exp_binop> x)
	{
		produce(make_s<ir_exp_binop>(
			x->op,
			map(x->e_left),
			map(x->e_right)
		));
	}
	
	void ir_desequencer::map(const s_ptr<const ir_exp_const> x)
	{
		produce(x);
	}
	
	void ir_desequencer::map(const s_ptr<const ir_exp_eseq> x)
	{
		map(x->s);
		produce(map(x->e));
	}
	
	void ir_desequencer::map(const s_ptr<const ir_exp_mem> x)
	{
		produce(make_s<ir_exp_mem>(map(x->e)));
	}
	
	void ir_desequencer::map(const s_ptr<const ir_exp_name> x)
	{
		produce(x);
	}
	
	void ir_desequencer::map(const s_ptr<const ir_exp_temp> x)
	{
		produce(x);
	}
	
	//Inherited from ir_stmt_transformer
	void ir_desequencer::map(const s_ptr<const ir_stmt_call> x)
	{
		std::vector<s_ptr<const ir_exp>> args;
		for(const auto arg : x->args)
			args.push_back(map(arg));
	
		stmts.push_back(make_s<ir_stmt_call>(
			map(x->e),
			args
		));
	}
	
	void ir_desequencer::map(const s_ptr<const ir_stmt_cjump> x)
	{
		stmts.push_back(make_s<ir_stmt_cjump>(
			x->r,
			map(x->e_left),
			map(x->e_right),
			x->l_left,
			x->l_right
		));
	}
	
	void ir_desequencer::map(const s_ptr<const ir_stmt_jump> x)
	{
		stmts.push_back(make_s<ir_stmt_jump>(
			map(x->e)
		));
	}
	
	void ir_desequencer::map(const s_ptr<const ir_stmt_label> x)
	{
		stmts.push_back(x);
	}
	
	void ir_desequencer::map(const s_ptr<const ir_stmt_move> x)
	{
		stmts.push_back(make_s<ir_stmt_move>(
			map(x->e_left),
			map(x->e_right)
		));
	}
	
	void ir_desequencer::map(const s_ptr<const ir_stmt_seq> x)
	{
		map(x->s_left);
		map(x->s_right);
	}
}

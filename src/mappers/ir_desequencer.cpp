#include "ir_desequencer.hpp"

#include "../ir/ir_exp_binop.hpp"
#include "../ir/ir_exp_const.hpp"
#include "../ir/ir_exp_eseq.hpp"
#include "../ir/ir_exp_mem.hpp"
#include "../ir/ir_exp_name.hpp"
#include "../ir/ir_exp_temp.hpp"

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
	
	s_ptr<const ir_exp> ir_desequencer::desequence(const s_ptr<const ir_exp> x)
	{
		x->map(*this);
		return acc;
	}
	
	std::list<s_ptr<const ir_stmt>> ir_desequencer::desequence(const s_ptr<const ir_stmt> x)
	{
		x->map(*this);
		return stmts;
	}
	
	//Inherited from ir_exp_transformer
	void ir_desequencer::map(const s_ptr<const ir_exp_binop> x)
	{
		produce(ir_exp_binop::create(
			x->op,
			desequence(x->e_left),
			desequence(x->e_right)
		));
	}
	
	void ir_desequencer::map(const s_ptr<const ir_exp_const> x)
	{
		produce(x);
	}
	
	void ir_desequencer::map(const s_ptr<const ir_exp_eseq> x)
	{
		desequence(x->s);
		produce(desequence(x->e));
	}
	
	void ir_desequencer::map(const s_ptr<const ir_exp_mem> x)
	{
		produce(ir_exp_mem::create(desequence(x->e)));
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
	void ir_desequencer::map(const s_ptr<const ir_stmt_cjump> x)
	{
		stmts.push_back(ir_stmt_cjump::create(
			x->r,
			desequence(x->e_left),
			desequence(x->e_right),
			x->l_left,
			x->l_right
		));
	}
	
	void ir_desequencer::map(const s_ptr<const ir_stmt_jump> x)
	{
		stmts.push_back(ir_stmt_jump::create(
			desequence(x->e)
		));
	}
	
	void ir_desequencer::map(const s_ptr<const ir_stmt_label> x)
	{
		stmts.push_back(x);
	}
	
	void ir_desequencer::map(const s_ptr<const ir_stmt_move> x)
	{
		stmts.push_back(ir_stmt_move::create(
			desequence(x->e_left),
			desequence(x->e_right)
		));
	}
	
	void ir_desequencer::map(const s_ptr<const ir_stmt_seq> x)
	{
		desequence(x->s_left);
		desequence(x->s_right);
	}
}

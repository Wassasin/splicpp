#include "ir_temp_allocator.hpp"

#include <stdexcept>

#include "../common/generic.hpp"

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
	void ir_temp_allocator::produce(s_ptr<const ir_exp> r)
	{
		acc = r;
	}
	
	s_ptr<const ir_exp> ir_temp_allocator::map(const s_ptr<const ir_exp> x)
	{
		x->map(*this);
		return acc;
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_stmt> x)
	{
		x->map(*this);
	}
	
	std::vector<s_ptr<const ir_stmt>> ir_temp_allocator::apply(const std::vector<s_ptr<const ir_stmt>>& stmts, const std::map<ir_temp, ir_temp>& reserved_temps, const std::vector<ir_temp>& scratch_temps)
	{
		std::map<ir_temp, ir_temp> tmap;
		for(std::pair<const ir_temp, const ir_temp> rt : reserved_temps)
			tmap[rt.first] = rt.second;
		
		//TODO create tmap
		
		ir_temp_allocator a(tmap);
		
		for(const auto stmt : stmts)
			stmt->map(a);
		
		return a.stmts;
	}

	//Inherited from ir_exp_transformer
	void ir_temp_allocator::map(const s_ptr<const ir_exp_binop> x)
	{
		produce(make_s<ir_exp_binop>(
			x->op,
			map(x->e_left),
			map(x->e_right)
		));
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_exp_const> x)
	{
		produce(x);
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_exp_eseq> x)
	{
		throw std::logic_error("ir_exp_eseq is not allowed in this stage; run the ir_desequencer first");
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_exp_mem> x)
	{
		produce(make_s<ir_exp_mem>(map(x->e)));
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_exp_name> x)
	{
		produce(x);
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_exp_temp> x)
	{
		produce(make_s<ir_exp_temp>(tmap.at(x->t)));
	}
	
	//Inherited from ir_stmt_transformer
	void ir_temp_allocator::map(const s_ptr<const ir_stmt_call> x)
	{
		throw std::logic_error("ir_stmt_call is not allowed in this stage; run the ir_call_transformer first");
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_stmt_cjump> x)
	{
		stmts.push_back(make_s<ir_stmt_cjump>(
			x->r,
			map(x->e_left),
			map(x->e_right),
			x->l_left,
			x->l_right
		));
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_stmt_jump> x)
	{
		stmts.push_back(make_s<ir_stmt_jump>(
			map(x->e)
		));
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_stmt_label> x)
	{
		stmts.push_back(x);
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_stmt_move> x)
	{
		stmts.push_back(make_s<ir_stmt_move>(
			map(x->e_left),
			map(x->e_right)
		));
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_stmt_seq> x)
	{
		throw std::logic_error("ir_stmt_seq is not allowed in this stage; run the ir_desequencer first");
	}
}

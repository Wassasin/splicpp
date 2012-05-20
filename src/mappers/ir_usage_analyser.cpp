#include "ir_usage_analyser.hpp"

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
	void ir_usage_analyser::analyse(const s_ptr<const ir_exp> x)
	{
		x->map(*this);
	}
	
	void ir_usage_analyser::analyse(const s_ptr<const ir_stmt> x)
	{
		x->map(*this);
	}
	
	std::vector<ir_usage_analyser::usage> ir_usage_analyser::analyse(const std::vector<s_ptr<const ir_stmt>>& stmts)
	{
		std::vector<usage> result;
		result.reserve(stmts.size());
	
		for(const auto stmt : stmts)
		{
			ir_usage_analyser a;
			a.analyse(stmt);
			result.push_back(a.u);
		}
		
		return result;
	}

	//Inherited from ir_exp_transformer
	void ir_usage_analyser::map(const s_ptr<const ir_exp_binop> x)
	{
		analyse(x->e_left);
		analyse(x->e_right);
	}
	
	void ir_usage_analyser::map(const s_ptr<const ir_exp_const>)
	{
		//Do nothing
	}
	
	void ir_usage_analyser::map(const s_ptr<const ir_exp_eseq>)
	{
		throw std::logic_error("ir_exp_eseq is not allowed in this stage; run the ir_desequencer first");
	}
	
	void ir_usage_analyser::map(const s_ptr<const ir_exp_mem> x)
	{
		analyse(x->e);
	}
	
	void ir_usage_analyser::map(const s_ptr<const ir_exp_name>)
	{
		//Do nothing
	}
	
	void ir_usage_analyser::map(const s_ptr<const ir_exp_temp> x)
	{
		if(!is_in<ir_temp>(x->t, u.used))
			u.used.push_back(x->t);
	}

	//Inherited from ir_stmt_transformer
	void ir_usage_analyser::map(const s_ptr<const ir_stmt_call> x)
	{
		analyse(x->e);
		
		for(const auto arg : x->args)
			analyse(arg);
	}
	
	void ir_usage_analyser::map(const s_ptr<const ir_stmt_cjump> x)
	{
		analyse(x->e_left);
		analyse(x->e_right);
	}
	
	void ir_usage_analyser::map(const s_ptr<const ir_stmt_jump> x)
	{
		analyse(x->e);
	}
	
	void ir_usage_analyser::map(const s_ptr<const ir_stmt_label>)
	{
		//Do nothing
	}
	
	void ir_usage_analyser::map(const s_ptr<const ir_stmt_move> x)
	{
		const s_ptr<const ir_exp_temp> e_left_temp(std::dynamic_pointer_cast<const ir_exp_temp>(x->e_left));
		
		if(e_left_temp == nullptr)
			analyse(x->e_left); //Does not set a temp, thus check if a temp is used to set anything else
		else
			u.defined = e_left_temp->t;
		
		analyse(x->e_right);
	}
	
	void ir_usage_analyser::map(const s_ptr<const ir_stmt_seq>)
	{
		throw std::logic_error("ir_stmt_seq is not allowed in this stage; run the ir_desequencer first");
	}
}

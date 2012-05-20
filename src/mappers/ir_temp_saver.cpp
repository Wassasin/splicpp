#include "ir_temp_saver.hpp"

#include <stdexcept>

#include "ir_desequencer.hpp"
#include "ir_liveness_analyser.hpp"

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
	void ir_temp_saver::standard(const s_ptr<const ir_stmt> x)
	{
		result.push_back(x);
	}
	
	std::vector<s_ptr<const ir_stmt>> ir_temp_saver::apply(const std::vector<s_ptr<const ir_stmt>>& stmts, const ircontext& c)
	{
		ir_temp_saver s(c);

		std::vector<ir_liveness_analyser::liveness> liveness = ir_liveness_analyser::analyse(stmts);
		
		for(size_t i = 0; i < stmts.size(); i++)
		{
			s.active_temps = liveness[i].live_out;
			stmts[i]->map(s);
		}
		
		return s.result;
	}

	void ir_temp_saver::map(const s_ptr<const ir_stmt_call> x)
	{
		const ir_label l_return = c.create_label();
		
		const s_ptr<const ir_exp> r_temp(make_s<ir_exp_temp>(c.create_temporary()));
		const s_ptr<const ir_exp> r_frame(make_s<ir_exp_temp>(c.frame_reg));
		const s_ptr<const ir_exp> r_stack(make_s<ir_exp_temp>(c.stack_reg));
		
		//Function call stack items
		std::vector<s_ptr<const ir_exp>> items;
		items.push_back(make_s<ir_exp_temp>(c.frame_reg));
		items.push_back(make_s<ir_exp_name>(l_return));
		for(const auto arg : x->args)
			items.push_back(arg);
		
		//Push active temporaries to stack
		std::vector<s_ptr<const ir_exp>> temps;
		for(const ir_temp t : active_temps)
			temps.push_back(make_s<ir_exp_temp>(t));
		
		s_ptr<const ir_stmt> r(ir_stmt::push(temps, c));
		
		//Prepare for function call, and call
		ir_stmt::cat(r, make_s<ir_stmt_move>(r_temp, r_stack));
		ir_stmt::cat(r, ir_stmt::push(items, c));
		ir_stmt::cat(r, make_s<ir_stmt_move>(r_frame, r_temp)); //Set old stack_ptr as frame_ptr
		ir_stmt::cat(r, make_s<ir_stmt_jump>(x->e));
		
		//Clean up after function has been called
		ir_stmt::cat(r, make_s<ir_stmt_label>(l_return));
		ir_stmt::cat(r, make_s<ir_stmt_move>(r_stack, r_frame)); //Clean up all deprecated entries on stack
		ir_stmt::cat(r, make_s<ir_stmt_move>(r_frame, make_s<ir_exp_mem>(r_frame))); //Set the old frame_ptr back
		
		//Re-activate temporaries
		for(size_t i = 0; i < temps.size(); i++)
			ir_stmt::cat(r, make_s<ir_stmt_move>(
				temps[i],
				ir_stmt::fetch_relative(temps.size() - i, c.stack_reg)
			));
		
		//Pop temporaries from stack
		ir_stmt::cat(r, ir_stmt::pop(temps.size(), c));
			
		for(const s_ptr<const ir_stmt> stmt : ir_desequencer::desequence(r))
			result.push_back(stmt);
	}

	void ir_temp_saver::map(const s_ptr<const ir_stmt_cjump> x)
	{
		standard(x);
	}
	
	void ir_temp_saver::map(const s_ptr<const ir_stmt_jump> x)
	{
		standard(x);
	}
	
	void ir_temp_saver::map(const s_ptr<const ir_stmt_label> x)
	{
		standard(x);
	}
	
	void ir_temp_saver::map(const s_ptr<const ir_stmt_move> x)
	{
		standard(x);
	}
	
	void ir_temp_saver::map(const s_ptr<const ir_stmt_seq>)
	{
		throw std::logic_error("ir_stmt_seq is not allowed in this stage; run the ir_desequencer first");
	}
}

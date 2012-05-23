#include "ssm_translator.hpp"

#include <stdexcept>
#include <sstream>

#include "ssm_instructions.hpp"
#include "ssm_tiles.hpp"

#include "../../ir/ir_stmt.hpp"

#include "../../mappers/ir_desequencer.hpp"
#include "../../mappers/ir_call_transformer.hpp"
#include "../../mappers/ir_frame_transformer.hpp"
#include "../../mappers/ir_liveness_analyser.hpp"
#include "../../mappers/ir_temp_allocator.hpp"

namespace splicpp
{
	void ssm_line::print(std::ostream& s) const
	{
		if(label)
			s << label.get() << ":\t";
		
		instruction->print(s);
		s << std::endl;
	}

	std::vector<s_ptr<const ssm_stmt_tile>> ssm_translator::initialize_stmts()
	{
		std::vector<s_ptr<const ssm_stmt_tile>> tiles;
		
		return tiles;
	}
	
	std::vector<s_ptr<const ssm_exp_tile>> ssm_translator::initialize_exps()
	{
		std::vector<s_ptr<const ssm_exp_tile>> tiles;
		
		return tiles;
	}

	std::vector<ssm_line> ssm_translator::translate(const s_ptr<const ir_stmt>& stmt, const ircontext& ir_c)
	{
		std::vector<std::shared_ptr<const splicpp::ir_stmt>> stmts;
		
		stmts = splicpp::ir_desequencer::desequence(stmt);
		stmts = splicpp::ir_call_transformer::apply(stmts, ir_c);
		
		stmts = splicpp::ir_temp_allocator::apply(stmts, context.register_map(ir_c), context.scratch_registers, ir_c);
		
		std::vector<ssm_line> result;
		for(const ir_frame& frame : splicpp::ir_frame_transformer::transform(stmts))
		{
			progress = 0;
			
			if(frame.stmts.size() == 0)
			{
				if(frame.label)
					result.push_back(ssm_line(frame.label.get(), make_s<ssm_nop>()));
				
				continue;
			}
			
			ssm_tresult_opt tresult = find_best_match(frame.stmts, 0);
			if(!tresult)
			{
				std::stringstream s;
				s << "Can not translate intermediate assembly, progress halted on" << std::endl;
				frame.stmts.at(progress)->print(s << '\t', 1);
				throw std::runtime_error(s.str());
			}
			
			const std::list<s_ptr<const ssm>>& instructions = tresult.get().fetch_instructions();
			
			auto ssm_i = instructions.cbegin();
			while(ssm_i != instructions.cend())
				if(ssm_i == instructions.cbegin() && frame.label) //First frame instruction with label
					result.push_back(ssm_line(frame.label.get(), *ssm_i));
				else
					result.push_back(ssm_line(*ssm_i));
		}
		
		return result;
	}
	
	ssm_tresult_opt ssm_translator::find_best_match(const std::vector<s_ptr<const ir_stmt>>& stmts, const size_t i)
	{
		if(i > progress)
			progress = i;
	
		ssm_tresult_opt result;
		for(const auto tile : stmt_tiles)
		{
			const ssm_tresult_opt& tmp_result = tile->try_match(stmts, i, *this);

			if(!tmp_result)
				continue;
		
			if(result && result.get().fetch_cost() < tmp_result.get().fetch_cost())
				continue;
			
			result = tmp_result;
		}
		
		return result;
	}
	
	ssm_tresult_opt ssm_translator::find_best_match(const s_ptr<const ir_exp> exp)
	{
		ssm_tresult_opt result;
		for(const auto tile : exp_tiles)
		{
			const ssm_tresult_opt& tmp_result = tile->try_match(exp, *this);

			if(!tmp_result)
				continue;
		
			if(result && result.get().fetch_cost() < tmp_result.get().fetch_cost())
				continue;
			
			result = tmp_result;
		}
		
		return result;
	}
}

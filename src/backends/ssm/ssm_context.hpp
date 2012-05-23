#ifndef SSM_CONTEXT_H
#define SSM_CONTEXT_H

#include <vector>
#include <map>

#include "../../ir/ircontext.hpp"

namespace splicpp
{
	class ssm_context
	{
		static std::vector<ir_temp> init_scratch()
		{
			std::vector<ir_temp> result;
			
			result.push_back(0x5);
			result.push_back(0x6);
			result.push_back(0x7);
			
			return result;
		}
	
	public:
		const ir_temp program_reg, stack_reg, frame_reg, return_reg, heap_reg;
		const std::vector<ir_temp> scratch_registers;
		
		ssm_context()
		: program_reg(0x0)
		, stack_reg(0x1)
		, frame_reg(0x2)
		, return_reg(0x3)
		, heap_reg(0x4)
		, scratch_registers(init_scratch())
		{}
		
		std::map<ir_temp, ir_temp> register_map(const ircontext& c) const
		{
			std::map<ir_temp, ir_temp> map;
			map[c.stack_reg] = stack_reg;
			map[c.frame_reg] = frame_reg;
			map[c.return_reg] = return_reg;
			map[c.heap_reg] = heap_reg;
		
			return map;
		}
	};
}

#endif

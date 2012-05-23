#include "ir_frame_transformer.hpp"

#include "../common/generic.hpp"

#include "ir_label_analyser.hpp"

namespace splicpp
{
	std::vector<ir_frame> ir_frame_transformer::transform(const std::vector<s_ptr<const ir_stmt>>& stmts)
	{
		const std::map<size_t, ir_label> labelmap = reverse<ir_label, size_t>(ir_label_analyser::analyse(stmts));
		
		std::vector<ir_frame> result;
		ir_frame current_frame;
		
		for(size_t i = 0; i < stmts.size(); i++)
		{
			const auto map_i = labelmap.find(i);
			if(map_i == labelmap.cend())
				current_frame.stmts.push_back(stmts.at(i));
			else
			{
				result.push_back(current_frame);
				current_frame = ir_frame(map_i->second);
			}
		}
		
		result.push_back(current_frame);
		
		return result;
	}
}

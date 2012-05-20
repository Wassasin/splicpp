#ifndef IR_LIVENESS_ANALYSER_H
#define IR_LIVENESS_ANALYSER_H

#include <vector>
#include <unordered_set>

#include "../common/typedefs.hpp"

#include "generic/ir_stmt_mapper.hpp"

namespace splicpp
{
	class ir_stmt;
	
	class ir_liveness_analyser
	{
	public:
		struct liveness
		{
			std::unordered_set<ir_temp> live_in, live_out;
			
			liveness()
			: live_in()
			, live_out()
			{}
		};
	
	private:
		ir_liveness_analyser()
		{}
		
	public:
		static std::vector<liveness> analyse(std::vector<s_ptr<const ir_stmt>> stmts);
	};
}

#endif

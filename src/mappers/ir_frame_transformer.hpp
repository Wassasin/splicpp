#ifndef IR_FRAME_TRANSFORMER_H
#define IR_FRAME_TRANSFORMER_H

#include <vector>
#include <boost/optional.hpp>

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_stmt;
	
	struct ir_frame
	{
		boost::optional<ir_label> label;
		std::vector<s_ptr<const ir_stmt>> stmts;
		
		ir_frame()
		: label()
		, stmts()
		{}
		
		ir_frame(const ir_label label)
		: label(label)
		, stmts()
		{}
	};
	
	class ir_frame_transformer
	{
		ir_frame_transformer()
		{}
	
	public:
		static std::vector<ir_frame> transform(const std::vector<s_ptr<const ir_stmt>>& stmts);
	};
}

#endif

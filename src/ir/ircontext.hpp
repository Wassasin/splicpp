#ifndef IRCONTEXT_H
#define IRCONTEXT_H

#include <map>

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ircontext
	{
		s_ptr<ir_label> lcount;
		s_ptr<ir_temp> tcount;
		
		std::map<sid, ir_label> memmap;
	public:
		const ir_temp heap_reg;
		const ir_temp frame_reg;
		
		const ir_label l_nil;
		
		ircontext()
		: lcount(new ir_label(0))
		, tcount(new ir_temp(0))
		, memmap()
		, heap_reg(create_temporary())
		, frame_reg(create_temporary())
		, l_nil(create_label())
		{}
		
		ir_label create_label();
		ir_temp create_temporary();
		ir_label fetch_label(const sid i) const;
	};
}

#endif

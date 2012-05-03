#ifndef IRCONTEXT_H
#define IRCONTEXT_H

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ircontext
	{
		ir_label lcount;
		ir_temp tcount;
	public:
		ircontext()
		: lcount(0)
		, tcount(0)
		{}
		
		ir_label create_label()
		{
			return lcount++;
		}
		
		ir_temp create_temporary()
		{
			return tcount++;
		}
	};
}

#endif

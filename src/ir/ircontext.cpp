#include "ircontext.hpp"

namespace splicpp
{
	ir_label ircontext::create_label()
	{
		return lcount++;
	}
	
	ir_temp ircontext::create_temporary()
	{
		return tcount++;
	}

	ir_label ircontext::fetch_label(const sid i) const
	{
		return memmap.at(i);
	}
}

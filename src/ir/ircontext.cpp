#include "ircontext.hpp"

namespace splicpp
{
	ir_label ircontext::create_label() const
	{
		return (*lcount)++;
	}
	
	ir_temp ircontext::create_temporary() const
	{
		return (*tcount)++;
	}

	s_ptr<const ir_exp> ircontext::fetch_memloc(const sid i) const
	{
		return memmap.at(i);
	}
	
	void ircontext::register_memloc(const sid i, const s_ptr<const ir_exp> e)
	{
		memmap[i] = e;
	}
}

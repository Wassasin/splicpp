#include "ir_stmt.hpp"

#include "ir_stmt_seq.hpp"

namespace splicpp
{
	void ir_stmt::cat(s_ptr<const ir_stmt>& r, const s_ptr<const ir_stmt> x)
	{
		r = ir_stmt_seq::create(r, x);
	}
}

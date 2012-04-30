#ifndef IR_STMT_SEQ_H
#define IR_STMT_SEQ_H

#include "ir_stmt.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_stmt_seq : public ir_stmt
	{
		const s_ptr<const ir_stmt> s_left, s_right;
		
	public:
		ir_stmt_seq(const s_ptr<const ir_stmt> s_left, const s_ptr<const ir_stmt> s_right)
		: s_left(s_left)
		, s_right(s_right)
		{}
	
		virtual void print(std::ostream& s, const uint tab) const;
	};
}

#endif

#ifndef IR_STMT_SEQ_H
#define IR_STMT_SEQ_H

#include "ir_stmt.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_stmt_seq : public ir_stmt
	{
	public:
		const s_ptr<const ir_stmt> s_left, s_right;
	
		ir_stmt_seq(const s_ptr<const ir_stmt> s_left, const s_ptr<const ir_stmt> s_right)
		: s_left(s_left)
		, s_right(s_right)
		{}
	
		virtual void transform(ir_stmt_transformer& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_stmt> create(const s_ptr<const ir_stmt> s_left, const s_ptr<const ir_stmt> s_right)
		{
			return s_ptr<const ir_stmt>(new ir_stmt_seq(s_left, s_right));
		}
	};
}

#endif

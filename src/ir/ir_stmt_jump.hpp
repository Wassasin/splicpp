#ifndef IR_STMT_JUMP_H
#define IR_STMT_JUMP_H

#include "ir_stmt.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp;

	class ir_stmt_jump : public ir_stmt
	{
		const s_ptr<const ir_exp> e;
		
	public:
		ir_stmt_jump(const s_ptr<const ir_exp> e)
		: e(e)
		{}
	
		virtual void print(std::ostream& s, const uint tab) const;
	};
}

#endif

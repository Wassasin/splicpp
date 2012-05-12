#ifndef IR_STMT_JUMP_H
#define IR_STMT_JUMP_H

#include "ir_stmt.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp;

	class ir_stmt_jump : public ir_stmt
	{
	public:
		const s_ptr<const ir_exp> e;
	
		ir_stmt_jump(const s_ptr<const ir_exp> e)
		: e(e)
		{}
		
		virtual void transform(ir_stmt_transformer& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_stmt> create(const s_ptr<const ir_exp> e)
		{
			return s_ptr<const ir_stmt>(new ir_stmt_jump(e));
		}
	};
}

#endif

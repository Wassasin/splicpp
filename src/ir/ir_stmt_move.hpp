#ifndef IR_STMT_MOVE_H
#define IR_STMT_MOVE_H

#include "ir_stmt.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp;

	class ir_stmt_move : public ir_stmt
	{
	public:
		const s_ptr<const ir_exp> e_left, e_right;
	
		ir_stmt_move(const s_ptr<const ir_exp> e_left, const s_ptr<const ir_exp> e_right)
		: e_left(e_left)
		, e_right(e_right)
		{}
	
		virtual void transform(ir_stmt_transformer& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_stmt> create(const s_ptr<const ir_exp> e_left, const s_ptr<const ir_exp> e_right)
		{
			return s_ptr<const ir_stmt>(new ir_stmt_move(e_left, e_right));
		}
	};
}

#endif

#ifndef IR_STMT_EXP_H
#define IR_STMT_EXP_H

#include "ir_stmt.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp;

	class ir_stmt_exp : public ir_stmt
	{
		const s_ptr<const ir_exp> e;
		
	public:
		ir_stmt_exp(const s_ptr<const ir_exp> e)
		: e(e)
		{}
	
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_stmt> create(const s_ptr<const ir_exp> e)
		{
			return s_ptr<const ir_stmt>(new ir_stmt_exp(e));
		}
	};
}

#endif

#ifndef IR_STMT_CJUMP_H
#define IR_STMT_CJUMP_H

#include "ir_stmt.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp;

	class ir_stmt_cjump : public ir_stmt
	{
	public:
		enum relop
		{
			eq,
			ne,
			lt,
			gt,
			le,
			ge,
			ult,
			ule,
			ugt,
			uge
		};
	
	private:
		const relop r;
		const s_ptr<const ir_exp> e_left, e_right;
		const ir_label l_left, l_right;
		
	public:
		ir_stmt_cjump(const relop r, const s_ptr<const ir_exp> e_left, const s_ptr<const ir_exp> e_right, const ir_label l_left, const ir_label l_right)
		: r(r)
		, e_left(e_left)
		, e_right(e_right)
		, l_left(l_left)
		, l_right(l_right)
		{}
	
		virtual void print(std::ostream& s, const uint tab) const;
	};
}

#endif

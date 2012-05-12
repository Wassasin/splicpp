#ifndef IR_EXP_ESEQ_H
#define IR_EXP_ESEQ_H

#include "ir_exp.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_stmt;

	class ir_exp_eseq : public ir_exp
	{
	public:
		const s_ptr<const ir_stmt> s;
		const s_ptr<const ir_exp> e;
	
		ir_exp_eseq(const s_ptr<const ir_stmt> s, const s_ptr<const ir_exp> e)
		: s(s)
		, e(e)
		{}
	
		virtual void transform(ir_exp_transformer& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_exp> create(const s_ptr<const ir_stmt> s, const s_ptr<const ir_exp> e)
		{
			return s_ptr<const ir_exp>(new ir_exp_eseq(s, e));
		}
	};
}

#endif

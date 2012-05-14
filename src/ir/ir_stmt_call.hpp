#ifndef IR_STMT_CALL_H
#define IR_STMT_CALL_H

#include "ir_stmt.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_exp;

	/*
	 * Call instruction to be translated to IR equivalent
	 *
	 * For the purpose of delaying the translation of call-routines
	 * When it is easier to do a usage-analysis on temporaries
	 * Specifically: to store used temporaries before a call, and restore them after the call
	 */
	
	class ir_stmt_call : public ir_stmt
	{
	public:
		const s_ptr<const ir_exp> e;
		const std::vector<s_ptr<const ir_exp>> args;
	
		ir_stmt_call(const s_ptr<const ir_exp> e, const std::vector<s_ptr<const ir_exp>> args)
		: e(e)
		, args(args)
		{}
		
		virtual void map(ir_stmt_mapper& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_stmt> create(const s_ptr<const ir_exp> e, const std::vector<s_ptr<const ir_exp>> args)
		{
			return s_ptr<const ir_stmt>(new ir_stmt_call(e, args));
		}
	};
}

#endif

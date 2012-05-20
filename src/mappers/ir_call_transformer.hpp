#ifndef IR_CALL_TRANSFORMER_H
#define IR_CALL_TRANSFORMER_H

#include <vector>
#include <unordered_set>

#include "../common/typedefs.hpp"
#include "../ir/ircontext.hpp"

#include "generic/ir_stmt_mapper.hpp"

namespace splicpp
{
	class ir_stmt;
	
	class ir_call_transformer : public ir_stmt_mapper
	{	
		const ircontext c;
		
		std::unordered_set<ir_temp> active_temps;
		std::vector<s_ptr<const ir_stmt>> result;
	
		ir_call_transformer(const ircontext& c)
		: c(c)
		, active_temps()
		, result()
		{}
		
		void standard(const s_ptr<const ir_stmt> x);
		
	public:
		static std::vector<s_ptr<const ir_stmt>> apply(const std::vector<s_ptr<const ir_stmt>>& stmts, const ircontext& c);
		
		virtual void map(const s_ptr<const ir_stmt_call> x);
		virtual void map(const s_ptr<const ir_stmt_cjump> x);
		virtual void map(const s_ptr<const ir_stmt_jump> x);
		virtual void map(const s_ptr<const ir_stmt_label> x);
		virtual void map(const s_ptr<const ir_stmt_move> x);
		virtual void map(const s_ptr<const ir_stmt_seq> x);
	};
}

#endif

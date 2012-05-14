#ifndef IR_DESEQUENCER_H
#define IR_DESEQUENCER_H

#include <vector>

#include "../common/typedefs.hpp"

#include "generic/ir_exp_mapper.hpp"
#include "generic/ir_stmt_mapper.hpp"

namespace splicpp
{
	class ir_stmt;
	class ir_exp;
	
	class ir_desequencer : public ir_exp_mapper, public ir_stmt_mapper
	{
		std::vector<s_ptr<const ir_stmt>> stmts;
		s_ptr<const ir_exp> acc;
		
		void produce(s_ptr<const ir_exp> r);
		
		s_ptr<const ir_exp> map(const s_ptr<const ir_exp> x);
		void map(const s_ptr<const ir_stmt> x);
		
		ir_desequencer()
		: stmts()
		, acc()
		{}
	public:
		static std::vector<s_ptr<const ir_stmt>> desequence(const s_ptr<const ir_stmt> x);
	
		virtual void map(const s_ptr<const ir_exp_binop> x);
		virtual void map(const s_ptr<const ir_exp_const> x);
		virtual void map(const s_ptr<const ir_exp_eseq> x);
		virtual void map(const s_ptr<const ir_exp_mem> x);
		virtual void map(const s_ptr<const ir_exp_name> x);
		virtual void map(const s_ptr<const ir_exp_temp> x);
		
		virtual void map(const s_ptr<const ir_stmt_call> x);
		virtual void map(const s_ptr<const ir_stmt_cjump> x);
		virtual void map(const s_ptr<const ir_stmt_jump> x);
		virtual void map(const s_ptr<const ir_stmt_label> x);
		virtual void map(const s_ptr<const ir_stmt_move> x);
		virtual void map(const s_ptr<const ir_stmt_seq> x);
	};
}

#endif

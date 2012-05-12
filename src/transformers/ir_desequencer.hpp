#ifndef IR_DESEQUENCER_H
#define IR_DESEQUENCER_H

#include <list>

#include "../common/typedefs.hpp"

#include "generic/ir_exp_transformer.hpp"
#include "generic/ir_stmt_transformer.hpp"

namespace splicpp
{
	class ir_stmt;
	class ir_exp;
	
	class ir_desequencer : public ir_exp_transformer, public ir_stmt_transformer
	{
		std::list<s_ptr<const ir_stmt>> stmts;
		s_ptr<const ir_exp> acc;
		
		void produce(s_ptr<const ir_exp> r);
	public:
		s_ptr<const ir_exp> transform(const s_ptr<const ir_exp> x);
		std::list<s_ptr<const ir_stmt>> transform(const s_ptr<const ir_stmt> x);
	
		virtual void transform(const s_ptr<const ir_exp_binop> x);
		virtual void transform(const s_ptr<const ir_exp_const> x);
		virtual void transform(const s_ptr<const ir_exp_eseq> x);
		virtual void transform(const s_ptr<const ir_exp_mem> x);
		virtual void transform(const s_ptr<const ir_exp_name> x);
		virtual void transform(const s_ptr<const ir_exp_temp> x);
		
		virtual void transform(const s_ptr<const ir_stmt_cjump> x);
		virtual void transform(const s_ptr<const ir_stmt_jump> x);
		virtual void transform(const s_ptr<const ir_stmt_label> x);
		virtual void transform(const s_ptr<const ir_stmt_move> x);
		virtual void transform(const s_ptr<const ir_stmt_seq> x);
	};
}

#endif

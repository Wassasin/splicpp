#ifndef IR_STMT_TRANSFORMER_H
#define IR_STMT_TRANSFORMER_H

namespace splicpp
{
	class ir_stmt_cjump;
	class ir_stmt_jump;
	class ir_stmt_label;
	class ir_stmt_move;
	class ir_stmt_seq;

	class ir_stmt_transformer
	{
	public:
		virtual void transform(const s_ptr<const ir_stmt_cjump> x) = 0;
		virtual void transform(const s_ptr<const ir_stmt_jump> x) = 0;
		virtual void transform(const s_ptr<const ir_stmt_label> x) = 0;
		virtual void transform(const s_ptr<const ir_stmt_move> x) = 0;
		virtual void transform(const s_ptr<const ir_stmt_seq> x) = 0;
	};
}

#endif

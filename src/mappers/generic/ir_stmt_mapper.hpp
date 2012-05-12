#ifndef IR_STMT_MAPPER_H
#define IR_STMT_MAPPER_H

namespace splicpp
{
	class ir_stmt_cjump;
	class ir_stmt_jump;
	class ir_stmt_label;
	class ir_stmt_move;
	class ir_stmt_seq;

	class ir_stmt_mapper
	{
	public:
		virtual ~ir_stmt_mapper() {}
		
		virtual void map(const s_ptr<const ir_stmt_cjump> x) = 0;
		virtual void map(const s_ptr<const ir_stmt_jump> x) = 0;
		virtual void map(const s_ptr<const ir_stmt_label> x) = 0;
		virtual void map(const s_ptr<const ir_stmt_move> x) = 0;
		virtual void map(const s_ptr<const ir_stmt_seq> x) = 0;
	};
}

#endif

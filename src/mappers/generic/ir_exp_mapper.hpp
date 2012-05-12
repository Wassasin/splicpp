#ifndef IR_EXP_MAPPER_H
#define IR_EXP_MAPPER_H

namespace splicpp
{
	class ir_exp_binop;
	class ir_exp_const;
	class ir_exp_eseq;
	class ir_exp_mem;
	class ir_exp_name;
	class ir_exp_temp;

	class ir_exp_mapper
	{
	public:
		virtual void map(const s_ptr<const ir_exp_binop> x) = 0;
		virtual void map(const s_ptr<const ir_exp_const> x) = 0;
		virtual void map(const s_ptr<const ir_exp_eseq> x) = 0;
		virtual void map(const s_ptr<const ir_exp_mem> x) = 0;
		virtual void map(const s_ptr<const ir_exp_name> x) = 0;
		virtual void map(const s_ptr<const ir_exp_temp> x) = 0;
	};
}

#endif

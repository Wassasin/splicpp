#ifndef IR_EXP_TRANSFORMER_H
#define IR_EXP_TRANSFORMER_H

namespace splicpp
{
	class ir_exp_binop;
	class ir_exp_const;
	class ir_exp_eseq;
	class ir_exp_mem;
	class ir_exp_name;
	class ir_exp_temp;

	class ir_exp_transformer
	{
	public:
		virtual void transform(const s_ptr<const ir_exp_binop> x) = 0;
		virtual void transform(const s_ptr<const ir_exp_const> x) = 0;
		virtual void transform(const s_ptr<const ir_exp_eseq> x) = 0;
		virtual void transform(const s_ptr<const ir_exp_mem> x) = 0;
		virtual void transform(const s_ptr<const ir_exp_name> x) = 0;
		virtual void transform(const s_ptr<const ir_exp_temp> x) = 0;
	};
}

#endif

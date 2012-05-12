#ifndef IR_GEN_TRANSFORMER_H
#define IR_GEN_TRANSFORMER_H

#include "ir_exp_transformer.hpp"
#include "ir_stmt_transformer.hpp"

namespace splicpp
{
	template <typename OUT_T>
	class ir_gen_transformer : public ir_exp_transformer, public ir_stmt_transformer
	{
	protected:
		OUT_T acc;
		
		void produce(OUT_T x)
		{
			acc = x;
		}
		
	public:
		OUT_T transform(const s_ptr<const ir_exp> x)
		{
			x->transform(*this);
			return acc;
		}
		
		OUT_T transform(const s_ptr<const ir_stmt> x)
		{
			x->transform(*this);
			return acc;
		}
	};
}

#endif

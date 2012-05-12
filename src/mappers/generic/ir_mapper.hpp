#ifndef IR_GEN_MAPPER_H
#define IR_GEN_MAPPER_H

#include "ir_exp_mapper.hpp"
#include "ir_stmt_mapper.hpp"

namespace splicpp
{
	template <typename OUT_T>
	class ir_mapper : public ir_exp_mapper, public ir_stmt_mapper
	{
	protected:
		OUT_T acc;
		
		void produce(OUT_T x)
		{
			acc = x;
		}
		
	public:
		OUT_T map(const s_ptr<const ir_exp> x)
		{
			x->transform(*this);
			return acc;
		}
		
		OUT_T map(const s_ptr<const ir_stmt> x)
		{
			x->transform(*this);
			return acc;
		}
	};
}

#endif

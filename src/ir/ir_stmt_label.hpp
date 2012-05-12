#ifndef IR_STMT_LABEL_H
#define IR_STMT_LABEL_H

#include "ir_stmt.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_stmt_label : public ir_stmt
	{
		const ir_label l;
		
	public:
		ir_stmt_label(const ir_label l)
		: l(l)
		{}
	
		virtual void transform(ir_stmt_transformer& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_stmt> create(const ir_label l)
		{
			return s_ptr<const ir_stmt>(new ir_stmt_label(l));
		}
	};
}

#endif

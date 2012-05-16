#ifndef IR_STMT_LABEL_H
#define IR_STMT_LABEL_H

#include "ir_stmt.hpp"

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ir_stmt_label : public ir_stmt
	{
	public:
		const ir_label l;
	
		ir_stmt_label(const ir_label l)
		: l(l)
		{}
	
		virtual void map(ir_stmt_mapper& t) const;
		virtual void print(std::ostream& s, const uint tab) const;
		
		static s_ptr<const ir_stmt> create(const ir_label l)
		{
			return make_s<ir_stmt_label>(l);
		}
	};
}

#endif

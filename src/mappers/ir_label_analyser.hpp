#ifndef IR_LABEL_ANALYSER_H
#define IR_LABEL_ANALYSER_H

#include <map>
#include <vector>

#include "../common/typedefs.hpp"

#include "generic/ir_stmt_mapper.hpp"

namespace splicpp
{
	class ir_stmt;
	
	class ir_label_analyser : public ir_stmt_mapper
	{
		size_t i;
		std::map<ir_label, size_t> index;
		
		ir_label_analyser()
		: i(0)
		, index()
		{}
	public:
		static std::map<ir_label, size_t> analyse(const std::vector<s_ptr<const ir_stmt>>& stmts);
		
		virtual void map(const s_ptr<const ir_stmt_cjump> x);
		virtual void map(const s_ptr<const ir_stmt_jump> x);
		virtual void map(const s_ptr<const ir_stmt_label> x);
		virtual void map(const s_ptr<const ir_stmt_move> x);
		virtual void map(const s_ptr<const ir_stmt_seq> x);
	};
}

#endif

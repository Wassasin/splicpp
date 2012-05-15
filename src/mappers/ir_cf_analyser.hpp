#ifndef IR_CF_ANALYSER_H
#define IR_CF_ANALYSER_H

#include <map>
#include <vector>

#include "../common/typedefs.hpp"
#include "../common/dgraph.hpp"

#include "generic/ir_stmt_mapper.hpp"

namespace splicpp
{
	class ir_stmt;
	
	class ir_cf_analyser : public ir_stmt_mapper
	{
		dgraph<size_t> g;
		size_t i;
		
		const size_t size;
		const std::map<ir_label, size_t> labelmap;
		
		ir_cf_analyser(const size_t size, const std::map<ir_label, size_t>& labelmap)
		: g()
		, i(0)
		, size(size)
		, labelmap(labelmap)
		{
			for(size_t i = 0; i < size; i++)
				g.add_vertex(i);
		}
		
		void standard();
		
	public:
		static dgraph<size_t> analyse(const std::vector<s_ptr<const ir_stmt>>& stmts, const std::map<ir_label, size_t>& labelmap);
		
		virtual void map(const s_ptr<const ir_stmt_call> x);
		virtual void map(const s_ptr<const ir_stmt_cjump> x);
		virtual void map(const s_ptr<const ir_stmt_jump> x);
		virtual void map(const s_ptr<const ir_stmt_label> x);
		virtual void map(const s_ptr<const ir_stmt_move> x);
		virtual void map(const s_ptr<const ir_stmt_seq> x);
	};
}

#endif

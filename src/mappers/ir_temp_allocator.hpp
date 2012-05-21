#ifndef IR_TEMP_ALLOCATOR_H
#define IR_TEMP_ALLOCATOR_H

#include <vector>
#include <map>

#include "../common/typedefs.hpp"
#include "../common/udgraph.hpp"

#include "generic/ir_exp_mapper.hpp"
#include "generic/ir_stmt_mapper.hpp"

namespace splicpp
{
	class ir_stmt;
	class ir_exp;
	class ircontext;

	class ir_temp_allocator : public ir_exp_mapper, public ir_stmt_mapper
	{
		const std::map<ir_temp, s_ptr<const ir_exp>> tmap;
	
		std::vector<s_ptr<const ir_stmt>> stmts;
		s_ptr<const ir_exp> acc;
		
		static std::vector<std::pair<ir_temp, bool>> simplify(udgraph<ir_temp> alloc_graph, const size_t k);
		
		void produce(s_ptr<const ir_exp> r);
		
		s_ptr<const ir_exp> map(const s_ptr<const ir_exp> x);
		void map(const s_ptr<const ir_stmt> x);
		
		ir_temp_allocator(const std::map<ir_temp, s_ptr<const ir_exp>>& tmap)
		: tmap(tmap)
		, stmts()
		, acc()
		{}
		
	public:
		static std::vector<s_ptr<const ir_stmt>> apply(const std::vector<s_ptr<const ir_stmt>>& stmts, const std::map<ir_temp, ir_temp>& reserved_temps, const std::vector<ir_temp>& scratch_temps, const ircontext& c);
	
		virtual void map(const s_ptr<const ir_exp_binop> x);
		virtual void map(const s_ptr<const ir_exp_const> x);
		virtual void map(const s_ptr<const ir_exp_eseq> x);
		virtual void map(const s_ptr<const ir_exp_mem> x);
		virtual void map(const s_ptr<const ir_exp_name> x);
		virtual void map(const s_ptr<const ir_exp_temp> x);
		
		virtual void map(const s_ptr<const ir_stmt_call> x);
		virtual void map(const s_ptr<const ir_stmt_cjump> x);
		virtual void map(const s_ptr<const ir_stmt_jump> x);
		virtual void map(const s_ptr<const ir_stmt_label> x);
		virtual void map(const s_ptr<const ir_stmt_move> x);
		virtual void map(const s_ptr<const ir_stmt_seq> x);
	};
}

#endif

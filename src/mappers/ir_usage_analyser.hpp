#ifndef IR_TEMP_ANALYSER_H
#define IR_TEMP_ANALYSER_H

#include <vector>
#include <boost/optional.hpp>

#include "../common/typedefs.hpp"

#include "generic/ir_exp_mapper.hpp"
#include "generic/ir_stmt_mapper.hpp"

namespace splicpp
{
	class ir_stmt;
	class ir_exp;

	class ir_usage_analyser : public ir_exp_mapper, public ir_stmt_mapper
	{
	public:
		struct usage
		{
			boost::optional<ir_temp> defined;
			std::vector<ir_temp> used;
			
			usage()
			: defined()
			, used()
			{}
		};
		
	private:
		usage u;
		
		ir_usage_analyser()
		: u()
		{}
		
		void analyse(const s_ptr<const ir_exp> x);
		void analyse(const s_ptr<const ir_stmt> x);
		
	public:
		static std::vector<usage> analyse(const std::vector<s_ptr<const ir_stmt>>& stmts);
	
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

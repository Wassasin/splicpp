#ifndef SSM_TRANSLATOR_H
#define SSM_TRANSLATOR_H

#include <vector>
#include <list>
#include <ostream>
#include <boost/optional.hpp>

#include "../../common/typedefs.hpp"

#include "ssm_context.hpp"
#include "ssm_tresult.hpp"

namespace splicpp
{
	class ir_stmt;
	class ircontext;

	struct ssm_stmt_tile;
	struct ssm_exp_tile;

	struct ssm;
	
	struct ssm_line
	{
		boost::optional<ir_label> label;
		s_ptr<const ssm> instruction;
		
		ssm_line(const ir_label label, const s_ptr<const ssm> instruction)
		: label(label)
		, instruction(instruction)
		{}
		
		ssm_line(const s_ptr<const ssm> instruction)
		: label()
		, instruction(instruction)
		{}
		
		void print(std::ostream& s) const;
	};
	
	class ssm_translator
	{
		std::vector<s_ptr<const ssm_stmt_tile>> stmt_tiles;
		std::vector<s_ptr<const ssm_exp_tile>> exp_tiles;
	
		static std::vector<s_ptr<const ssm_stmt_tile>> initialize_stmts();
		static std::vector<s_ptr<const ssm_exp_tile>> initialize_exps();
	
		size_t progress;
	public:
		const ssm_context context;
		
		ssm_translator()
		: stmt_tiles(initialize_stmts())
		, exp_tiles(initialize_exps())
		, progress(0)
		, context()
		{}
	
		std::vector<ssm_line> translate(const s_ptr<const ir_stmt>& stmt, const ircontext& c);
		
		ssm_tresult_opt find_best_match(const std::vector<s_ptr<const ir_stmt>>& stmts, const size_t i);
		ssm_tresult_opt find_best_match(const s_ptr<const ir_exp> exp);
	};
}

#endif

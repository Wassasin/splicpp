#ifndef SSM_TILES_H
#define SSM_TILES_H

#include <vector>

#include "../../common/typedefs.hpp"

#include "ssm_tresult.hpp"

namespace splicpp
{
	class ir_stmt;
	class ir_exp;
	class ssm_translator;

	struct ssm_stmt_tile
	{
		virtual ssm_tresult_opt try_match(const std::vector<s_ptr<const ir_stmt>>& stmts, const size_t i, const ssm_translator& t) const = 0;
	};
	
	struct ssm_exp_tile
	{
		virtual ssm_tresult_opt try_match(const s_ptr<const ir_exp> exp, const ssm_translator& t) const = 0;
	};
}

#endif

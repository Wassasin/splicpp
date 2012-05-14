#include "ir_cf_analyser.hpp"

#include <stdexcept>

#include "../ir/ir_stmt_call.hpp"
#include "../ir/ir_stmt_cjump.hpp"
#include "../ir/ir_stmt_jump.hpp"
#include "../ir/ir_stmt_label.hpp"
#include "../ir/ir_stmt_move.hpp"
#include "../ir/ir_stmt_seq.hpp"

#include "../ir/ir_exp_name.hpp"

namespace splicpp
{
	void ir_cf_analyser::standard()
	{
		if(i+1 < size)
			g.add_edge(i, i+1);
	}
	
	cfgraph ir_cf_analyser::analyse(const std::vector<s_ptr<const ir_stmt>>& stmts, const std::map<ir_label, size_t>& labelmap)
	{
		ir_cf_analyser a(stmts.size(), labelmap);
		
		for(const auto stmt : stmts)
		{
			stmt->map(a);
			a.i++;
		}
		
		return a.g;
	}

	void ir_cf_analyser::map(const s_ptr<const ir_stmt_call>)
	{
		standard(); //Unexpected, but function calls are not followed at this stage. When it is translated to lower-level IR this might be useful.
	}

	void ir_cf_analyser::map(const s_ptr<const ir_stmt_cjump> x)
	{
		g.add_edge(i, labelmap.at(x->l_left));
		g.add_edge(i, labelmap.at(x->l_right));
	}
	
	void ir_cf_analyser::map(const s_ptr<const ir_stmt_jump> x)
	{
		const s_ptr<const ir_exp_name> tmp_name(std::dynamic_pointer_cast<const ir_exp_name>(x->e));
		if(tmp_name != nullptr) //If it is something else, static analysis is not useful (and in most cases not possible)
			g.add_edge(i, labelmap.at(tmp_name->l));
	}
	
	void ir_cf_analyser::map(const s_ptr<const ir_stmt_label>)
	{
		standard();
	}
	
	void ir_cf_analyser::map(const s_ptr<const ir_stmt_move>)
	{
		standard();
	}
	
	void ir_cf_analyser::map(const s_ptr<const ir_stmt_seq>)
	{
		throw std::logic_error("ir_stmt_seq is not allowed in this stage; run the ir_desequencer first");
	}
}

#include "ir_label_analyser.hpp"

#include <stdexcept>

#include "../ir/ir_stmt_cjump.hpp"
#include "../ir/ir_stmt_jump.hpp"
#include "../ir/ir_stmt_label.hpp"
#include "../ir/ir_stmt_move.hpp"
#include "../ir/ir_stmt_seq.hpp"

namespace splicpp
{
	std::map<ir_label, size_t> ir_label_analyser::analyse(const std::vector<s_ptr<const ir_stmt>>& stmts)
	{
		ir_label_analyser a;
		
		for(const auto stmt : stmts)
		{
			stmt->map(a);
			a.i++;
		}
		
		return a.index;
	}

	void ir_label_analyser::map(const s_ptr<const ir_stmt_call>)
	{
		//Do nothing
	}

	void ir_label_analyser::map(const s_ptr<const ir_stmt_cjump>)
	{
		//Do nothing
	}
	
	void ir_label_analyser::map(const s_ptr<const ir_stmt_jump>)
	{
		//Do nothing
	}
	
	void ir_label_analyser::map(const s_ptr<const ir_stmt_label> x)
	{
		if(index.find(x->l) != index.end())
			throw std::logic_error("Multiple occurences of label found");
		
		index[x->l] = i;
	}
	
	void ir_label_analyser::map(const s_ptr<const ir_stmt_move>)
	{
		//Do nothing
	}
	
	void ir_label_analyser::map(const s_ptr<const ir_stmt_seq>)
	{
		throw std::logic_error("ir_stmt_seq is not allowed in this stage; run the ir_desequencer first");
	}
}

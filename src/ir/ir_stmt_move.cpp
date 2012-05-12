#include "ir_stmt_move.hpp"

#include "ir_exp.hpp"

#include "../transformers/generic/ir_stmt_transformer.hpp"

namespace splicpp
{
	void ir_stmt_move::transform(ir_stmt_transformer& t) const
	{
		t.transform(std::static_pointer_cast<const ir_stmt_move>(shared_from_this()));
	}
	
	void ir_stmt_move::print(std::ostream& s, const uint tab) const
	{
		s << "MOVE(";
		
		print_newline(s, tab+1);
		e_left->print(s, tab+1);
		s << ",";
		
		print_newline(s, tab+1);
		e_right->print(s, tab+1);
			
		print_newline(s, tab);
		s << ")";
	}
}

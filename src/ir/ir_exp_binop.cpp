#include "ir_exp_binop.hpp"

#include "../transformers/generic/ir_exp_transformer.hpp"

namespace splicpp
{
	void ir_exp_binop::transform(ir_exp_transformer& t) const
	{
		t.transform(std::static_pointer_cast<const ir_exp_binop>(shared_from_this()));
	}

	void ir_exp_binop::print(std::ostream& s, const uint tab) const
	{
		s << "BINOP(";
		
		print_newline(s, tab+1);
		s << op << ",";
		
		print_newline(s, tab+1);
		e_left->print(s, tab+1);
		
		s << ",";
		print_newline(s, tab+1);
		e_right->print(s, tab+1);
		
		print_newline(s, tab);
		s << ")";
	}
}

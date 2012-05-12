#include "ir_exp_binop.hpp"

#include "../mappers/generic/ir_exp_mapper.hpp"

namespace splicpp
{
	std::string ir_exp_binop::binop_to_string(const binop op)
	{
		switch(op)
		{
			case op_plus:
				return "+";
			case op_minus:
				return "-";
			case op_mul:
				return "*";
			case op_div:
				return "/";
			case op_mod:
				return "%";
			case op_and:
				return "&";
			case op_or:
				return "|";
			case op_lshift:
				return "<<";
			case op_rshift:
				return ">>";
			case op_arshift:
				return "C>>";
			case op_xor:
				return "^";
			default:
				return "unknown";
		}
	}

	void ir_exp_binop::map(ir_exp_mapper& t) const
	{
		t.map(std::static_pointer_cast<const ir_exp_binop>(shared_from_this()));
	}

	void ir_exp_binop::print(std::ostream& s, const uint tab) const
	{
		s << binop_to_string(op) << "(";
		
		print_newline(s, tab+1);
		e_left->print(s, tab+1);
		
		s << ",";
		print_newline(s, tab+1);
		e_right->print(s, tab+1);
		
		print_newline(s, tab);
		s << ")";
	}
}

#include "ast_type.hpp"

namespace splicpp
{
	/* ast_type_int */
	ast_type::ast_type_type ast_type_int::type() const
	{
		return type_int;
	}
	
	void ast_type_int::pretty_print(std::ostream& s, const uint) const
	{
		s << "Int";
	}
	
	/* ast_type_bool */
	
	ast_type::ast_type_type ast_type_bool::type() const
	{
		return type_bool;
	}
	
	void ast_type_bool::pretty_print(std::ostream& s, const uint) const
	{
		s << "Bool";
	}
	
	/* ast_type_tuple */
	
	ast_type::ast_type_type ast_type_tuple::type() const
	{
		return type_tuple;
	}
	
	void ast_type_tuple::pretty_print(std::ostream& s, const uint tab) const
	{
		s << "(";
		t_left->pretty_print(s, tab);
		s << ", ";
		t_right->pretty_print(s, tab);
		s << ")";
	}
	
	/* ast_type_array */
	
	ast_type::ast_type_type ast_type_array::type() const
	{
		return type_array;
	}
	
	void ast_type_array::pretty_print(std::ostream& s, const uint tab) const
	{
		s << "[";
		t->pretty_print(s, tab);
		s << "]";
	}
	
	/* ast_type_id */
	
	ast_type::ast_type_type ast_type_id::type() const
	{
		return type_id;
	}
	
	void ast_type_id::pretty_print(std::ostream& s, const uint tab) const
	{
		id->pretty_print(s, tab);
	}
}

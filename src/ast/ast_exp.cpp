#include "ast_exp.hpp"

#include "ast_id.hpp"
#include "ast_fun_call.hpp"

namespace splicpp
{
	/* ast_exp_id */
	
	void ast_exp_id::assign_ids(const varcontext& c)
	{
		id->assign_ids(c);
	}
	
	ast_exp::ast_exp_type ast_exp_id::type() const
	{
		return type_id;
	}
	
	void ast_exp_id::pretty_print(std::ostream& s, const uint tab) const
	{
		id->pretty_print(s, tab);
	}
	
	/* ast_exp_op2 */
	
	ast_exp_op2::op_type ast_exp_op2::optype() const
	{
		return t;
	}
	
	void ast_exp_op2::assign_ids(const varcontext& c)
	{
		e_left->assign_ids(c);
		e_right->assign_ids(c);
	}
	
	ast_exp::ast_exp_type ast_exp_op2::type() const
	{
		return type_op2;
	}
	
	void ast_exp_op2::pretty_print(std::ostream& s, const uint tab) const
	{
		e_left->pretty_print(s, tab);
		s << string_map[t];
		e_right->pretty_print(s, tab);
	}
	
	/* ast_exp_negation */
	
	void ast_exp_negation::assign_ids(const varcontext& c)
	{
		exp->assign_ids(c);
	}
	
	ast_exp::ast_exp_type ast_exp_negation::type() const
	{
		return type_negation;
	}
	
	void ast_exp_negation::pretty_print(std::ostream& s, const uint tab) const
	{
		s << '!';
		exp->pretty_print(s, tab);
	}
	
	/* ast_exp_int */
	
	void ast_exp_int::assign_ids(const varcontext&)
	{
		//Empty
	}
	
	ast_exp::ast_exp_type ast_exp_int::type() const
	{
		return type_int;
	}
	
	void ast_exp_int::pretty_print(std::ostream& s, const uint) const
	{
		s << i;
	}
	
	/* ast_exp_bool */
	
	void ast_exp_bool::assign_ids(const varcontext&)
	{
		//Empty
	}
	
	ast_exp::ast_exp_type ast_exp_bool::type() const
	{
		return type_bool;
	}
	
	void ast_exp_bool::pretty_print(std::ostream& s, const uint) const
	{
		if(b)
			s << "True";
		else
			s << "False";
	}
	
	/* ast_exp_exp */
	
	void ast_exp_exp::assign_ids(const varcontext& c)
	{
		exp->assign_ids(c);
	}
	
	ast_exp::ast_exp_type ast_exp_exp::type() const
	{
		return type_exp;
	}
	
	void ast_exp_exp::pretty_print(std::ostream& s, const uint tab) const
	{
		s << '(';
		exp->pretty_print(s, tab);
		s << ')';
	}
	
	/* ast_exp_fun_call */
	
	void ast_exp_fun_call::assign_ids(const varcontext& cvar)
	{
		c->assign_ids(cvar);
	}
	
	ast_exp::ast_exp_type ast_exp_fun_call::type() const
	{
		return type_fun_call;
	}
	
	void ast_exp_fun_call::pretty_print(std::ostream& s, const uint tab) const
	{
		c->pretty_print(s, tab);
	}
	
	/* ast_exp_nil */
	
	void ast_exp_nil::assign_ids(const varcontext&)
	{
		//Empty
	}
	
	ast_exp::ast_exp_type ast_exp_nil::type() const
	{
		return type_nil;
	}
	
	void ast_exp_nil::pretty_print(std::ostream& s, const uint) const
	{
		s << "[]";
	}
	
	/* ast_exp_tuple */
	
	void ast_exp_tuple::assign_ids(const varcontext& c)
	{
		e_left->assign_ids(c);
		e_right->assign_ids(c);
	}
	
	ast_exp::ast_exp_type ast_exp_tuple::type() const
	{
		return type_tuple;
	}
	
	void ast_exp_tuple::pretty_print(std::ostream& s, const uint tab) const
	{
		s << '(';
		e_left->pretty_print(s, tab);
		s << ", ";
		e_right->pretty_print(s, tab);
		s << ')';
	}
}

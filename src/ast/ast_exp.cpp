#include "ast_exp.hpp"

#include "ast_id.hpp"
#include "ast_fun_call.hpp"

#include "../typing/typecontext.hpp"
#include "../typing/types/sl_type.hpp"
#include "../typing/types/sl_type_int.hpp"
#include "../typing/types/sl_type_bool.hpp"
#include "../typing/types/sl_type_array.hpp"
#include "../typing/types/sl_type_unbound.hpp"
#include "../typing/types/sl_type_tuple.hpp"

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
	
	substitution ast_exp_id::infer_type(const typecontext& c, const std::shared_ptr<sl_type> t) const
	{
		return id->infer_type(c, t);
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
	
	substitution ast_exp_op2::infer_type(const typecontext& c, const std::shared_ptr<sl_type> t) const
	{
		std::shared_ptr<sl_type> t1, t2, r;
		switch(optype())
		{
			case op_plus:
			case op_minus:
			case op_times:
			case op_divides:
			case op_mod:
			{
				t1 = decltype(t1)(new sl_type_int());
				t2 = decltype(t2)(new sl_type_int());
				r = decltype(r)(new sl_type_int());
				break;
			}
			
			case op_eq:
			case op_lesser:
			case op_greater:
			case op_leq:
			case op_geq:
			case op_neq:
			{
				t1 = decltype(t1)(new sl_type_int());
				t2 = decltype(t2)(new sl_type_int());
				r = decltype(r)(new sl_type_bool());
				break;
			}	
			
			case op_conjunction:
			case op_disjunction:
			{
				t1 = decltype(t1)(new sl_type_bool());
				t2 = decltype(t2)(new sl_type_bool());
				r = decltype(r)(new sl_type_bool());
				break;
			}
			
			case op_cons:
			{
				t1 = std::static_pointer_cast<sl_type>(c.create_fresh());
				t2 = decltype(t2)(new sl_type_array(t1));
				r = decltype(r)(new sl_type_array(t1));
				break;
			}
		}
		
		const substitution s1 = e_left->infer_type(c, t1);
		const substitution s2 = e_right->infer_type(c.apply(s1), t2).composite(s1);
		
		return t->apply(s2)->unify(r).composite(s2);
		
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
	
	substitution ast_exp_negation::infer_type(const typecontext& c, const std::shared_ptr<sl_type> t) const
	{
		const std::shared_ptr<sl_type> b(new sl_type_bool());
		const substitution s = exp->infer_type(c, b);
		return t->apply(s)->unify(b).composite(s);
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
	
	substitution ast_exp_int::infer_type(const typecontext&, const std::shared_ptr<sl_type> t) const
	{
		return t->unify(std::shared_ptr<sl_type>(new sl_type_int()));
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
	
	substitution ast_exp_bool::infer_type(const typecontext&, const std::shared_ptr<sl_type> t) const
	{
		return t->unify(std::shared_ptr<sl_type>(new sl_type_bool()));
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
	
	substitution ast_exp_exp::infer_type(const typecontext& c, const std::shared_ptr<sl_type> t) const
	{
		return exp->infer_type(c, t);
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
	
	substitution ast_exp_fun_call::infer_type(const typecontext& c, const std::shared_ptr<sl_type> t) const
	{
		return this->c->infer_type(c, t);
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
	
	substitution ast_exp_nil::infer_type(const typecontext& c, const std::shared_ptr<sl_type> t) const
	{
		return t->unify(std::shared_ptr<sl_type>(new sl_type_array(c.create_fresh())));
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
	
	substitution ast_exp_tuple::infer_type(const typecontext& c, const std::shared_ptr<sl_type> t) const
	{
		const std::shared_ptr<sl_type> a1 = c.create_fresh();
		const std::shared_ptr<sl_type> a2 = c.create_fresh();
		
		const std::shared_ptr<sl_type> r(new sl_type_tuple(a1, a2));
		
		const substitution s1 = e_left->infer_type(c, a1);
		const substitution s2 = e_right->infer_type(c.apply(s1), a2).composite(s1);
	
		return t->apply(s2)->unify(r->apply(s2)).composite(s2);
	}
}

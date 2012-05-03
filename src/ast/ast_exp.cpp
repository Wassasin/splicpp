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

#include "../ir/ircontext.hpp"

#include "../ir/ir_exp_binop.hpp"
#include "../ir/ir_exp_const.hpp"
#include "../ir/ir_exp_eseq.hpp"
#include "../ir/ir_exp_name.hpp"
#include "../ir/ir_exp_temp.hpp"

#include "../ir/ir_stmt_cjump.hpp"
#include "../ir/ir_stmt_jump.hpp"
#include "../ir/ir_stmt_label.hpp"
#include "../ir/ir_stmt_move.hpp"

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
	
	substitution ast_exp_id::infer_type(const typecontext& c, const s_ptr<const sl_type> t) const
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
	
	substitution ast_exp_op2::infer_type(const typecontext& c, const s_ptr<const sl_type> t) const
	{
		s_ptr<const sl_type> t1, t2, r;
		switch(optype())
		{
			case op_plus:
			case op_minus:
			case op_times:
			case op_divides:
			case op_mod:
			{
				t1 = s_ptr<const sl_type>(new sl_type_int(e_left->sl));
				t2 = s_ptr<const sl_type>(new sl_type_int(e_right->sl));
				r = s_ptr<const sl_type>(new sl_type_int(sl));
				break;
			}
			
			case op_eq:
			case op_lesser:
			case op_greater:
			case op_leq:
			case op_geq:
			case op_neq:
			{
				t1 = s_ptr<const sl_type>(new sl_type_int(e_left->sl));
				t2 = s_ptr<const sl_type>(new sl_type_int(e_right->sl));
				r = s_ptr<const sl_type>(new sl_type_bool(sl));
				break;
			}	
			
			case op_conjunction:
			case op_disjunction:
			{
				t1 = s_ptr<const sl_type>(new sl_type_bool(e_left->sl));
				t2 = s_ptr<const sl_type>(new sl_type_bool(e_right->sl));
				r = s_ptr<const sl_type>(new sl_type_bool(sl));
				break;
			}
			
			case op_cons:
			{
				t1 = std::static_pointer_cast<const sl_type>(c.create_fresh(e_left->sl));
				t2 = s_ptr<const sl_type>(new sl_type_array(t1, e_right->sl));
				r = s_ptr<const sl_type>(new sl_type_array(t1, sl));
				break;
			}
		}	
		
		const substitution s1 = e_left->infer_type(c, t1);
		const substitution s2 = e_right->infer_type(c.apply(s1), t2->apply(s1)).composite(s1);
		
		return t->apply(s2)->unify(r->apply(s2)).composite(s2);
	}
	
	ir_exp_binop::binop translate_int_op(const ast_exp_op2::op_type o)
	{
		switch(o)
		{
			case ast_exp_op2::op_plus:
				return ir_exp_binop::op_plus;
			case ast_exp_op2::op_minus:
				return ir_exp_binop::op_minus;
			case ast_exp_op2::op_times:
				return ir_exp_binop::op_mul;
			case ast_exp_op2::op_divides:
				return ir_exp_binop::op_div;
			case ast_exp_op2::op_mod:
				return ir_exp_binop::op_mod;
			case ast_exp_op2::op_conjunction:
				return ir_exp_binop::op_and;
			case ast_exp_op2::op_disjunction:
				return ir_exp_binop::op_or;
			default:
				throw std::logic_error("Unknown ast_exp_op2::op_type");
		}
	}
	
	ir_stmt_cjump::relop translate_bool_op(const ast_exp_op2::op_type o)
	{
		switch(o)
		{
			case ast_exp_op2::op_eq:
				return ir_stmt_cjump::op_eq;
			case ast_exp_op2::op_lesser:
				return ir_stmt_cjump::op_lt;
			case ast_exp_op2::op_greater:
				return ir_stmt_cjump::op_gt;
			case ast_exp_op2::op_leq:
				return ir_stmt_cjump::op_le;
			case ast_exp_op2::op_geq:
				return ir_stmt_cjump::op_ge;
			case ast_exp_op2::op_neq:
				return ir_stmt_cjump::op_ne;
			default:
				throw std::logic_error("Unknown ast_exp_op2::op_type");
		}
	}
	
	s_ptr<const ir_exp> ast_exp_op2::translate(ircontext& c) const
	{
		switch(optype())
		{
			case op_plus:
			case op_minus:
			case op_times:
			case op_divides:
			case op_mod:
			case op_conjunction:
			case op_disjunction:
				return ir_exp_binop::create(
					translate_int_op(optype()),
					e_left->translate(c),
					e_right->translate(c)
				);
			case op_eq:
			case op_lesser:
			case op_greater:
			case op_leq:
			case op_geq:
			case op_neq:
			{
				ir_temp t = c.create_temporary();
				ir_label l_true = c.create_label();
				ir_label l_false = c.create_label();
				ir_label l_done = c.create_label();
				
				s_ptr<const ir_stmt> r(ir_stmt_cjump::create(
					translate_bool_op(optype()),
					e_left->translate(c),
					e_right->translate(c),
					l_true,
					l_false
				));
				
				ir_stmt::cat(r, ir_stmt_label::create(l_true));
				ir_stmt::cat(r, ir_stmt_move::create(
					ir_exp_temp::create(t),
					ir_exp_const::create(true)
				));
				ir_stmt::cat(r, ir_stmt_jump::create(ir_exp_name::create(l_done)));
				ir_stmt::cat(r, ir_stmt_label::create(l_false));
				ir_stmt::cat(r, ir_stmt_move::create(
					ir_exp_temp::create(t),
					ir_exp_const::create(false)
				));
				ir_stmt::cat(r, ir_stmt_label::create(l_done));
				
				return ir_exp_eseq::create(r, ir_exp_temp::create(t));
			}
			case op_cons:
			{
				//TODO
			}
		}
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
	
	substitution ast_exp_negation::infer_type(const typecontext& c, const s_ptr<const sl_type> t) const
	{
		const s_ptr<const sl_type> b(new sl_type_bool(sl));
		const substitution s = exp->infer_type(c, b);
		return t->apply(s)->unify(b).composite(s);
	}
	
	s_ptr<const ir_exp> ast_exp_negation::translate(ircontext& c) const
	{
		return ir_exp_binop::create(
			ir_exp_binop::op_xor,
			exp->translate(c),
			ir_exp_const::create(true)
		);
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
	
	substitution ast_exp_int::infer_type(const typecontext&, const s_ptr<const sl_type> t) const
	{
		return t->unify(s_ptr<const sl_type>(new sl_type_int(sl)));
	}
	
	s_ptr<const ir_exp> ast_exp_int::translate(ircontext&) const
	{
		return ir_exp_const::create(i);
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
	
	substitution ast_exp_bool::infer_type(const typecontext&, const s_ptr<const sl_type> t) const
	{
		return t->unify(s_ptr<const sl_type>(new sl_type_bool(sl)));
	}
	
	s_ptr<const ir_exp> ast_exp_bool::translate(ircontext&) const
	{
		return ir_exp_const::create(b);
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
	
	substitution ast_exp_exp::infer_type(const typecontext& c, const s_ptr<const sl_type> t) const
	{
		return exp->infer_type(c, t);
	}
	
	s_ptr<const ir_exp> ast_exp_exp::translate(ircontext& c) const
	{
		return exp->translate(c);
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
	
	substitution ast_exp_fun_call::infer_type(const typecontext& c, const s_ptr<const sl_type> t) const
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
	
	substitution ast_exp_nil::infer_type(const typecontext& c, const s_ptr<const sl_type> t) const
	{
		return t->unify(s_ptr<const sl_type>(new sl_type_array(c.create_fresh(sl), sl)));
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
	
	substitution ast_exp_tuple::infer_type(const typecontext& c, const s_ptr<const sl_type> t) const
	{
		const s_ptr<const sl_type> a1 = c.create_fresh(sl);
		const s_ptr<const sl_type> a2 = c.create_fresh(sl);
		
		const s_ptr<const sl_type> r(new sl_type_tuple(a1, a2, sl));
		
		const substitution s1 = e_left->infer_type(c, a1);
		const substitution s2 = e_right->infer_type(c.apply(s1), a2).composite(s1);
	
		return t->apply(s2)->unify(r->apply(s2)).composite(s2);
	}
}

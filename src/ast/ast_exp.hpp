#ifndef AST_EXP_H
#define AST_EXP_H

#include <boost/array.hpp>

#include "ast.hpp"

#include "ast_id.hpp"
#include "ast_fun_call.hpp"

namespace splicpp
{
	class ast_fun_call;

	class ast_exp : public ast
	{
	public:
		enum ast_exp_type
		{
			type_id,
			type_op2,
			type_negation,
			type_int,
			type_bool,
			type_exp,
			type_fun_call,
			type_nil,
			type_tuple
		};
		
		virtual ast_exp_type type() const = 0;
		virtual void pretty_print(std::ostream& s) const = 0;
	};
	
	class ast_exp_id : public ast_exp
	{
		std::shared_ptr<ast_id> id;
	
	public:
		ast_exp_id(__decltype(id) id)
		: id(id)
		{}
	
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s) const;
	};
	
	class ast_exp_op2 : public ast_exp
	{
		static const size_t OP_TYPE_LENGTH = 14;
	
	public:
		enum op_type
		{
			op_plus, op_minus, op_times, op_divides, op_mod, op_eq, op_lesser, op_greater, op_leq, op_geq, op_neq, op_conjunction, op_disjunction, op_cons
		};
	
	private:
		std::shared_ptr<ast_exp> e_left, e_right;
		op_type t;
		
		const boost::array<const std::string, OP_TYPE_LENGTH> string_map;
	public:
		ast_exp_op2(const __decltype(e_left) e_left, const op_type t, const __decltype(e_right) e_right)
		: e_left(e_left)
		, e_right(e_right)
		, t(t)
		, string_map({ { "+", "-", "*", "/", "%", "==", "<", ">", "<=", ">=", "!=", "&&", "||", ":" } })
		{}
	
		op_type optype() const;
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s) const;
	};
	
	class ast_exp_negation : public ast_exp
	{
		std::shared_ptr<ast_exp> exp;
	
	public:
		ast_exp_negation(__decltype(exp) exp)
		: exp(exp)
		{}
	
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s) const;
	};
	
	class ast_exp_int : public ast_exp
	{
		int i;
	
	public:
		ast_exp_int(__decltype(i) i)
		: i(i)
		{}
	
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s) const;
	};
	
	class ast_exp_bool : public ast_exp
	{
		bool b;
	
	public:
		ast_exp_bool(__decltype(b) b)
		: b(b)
		{}
	
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s) const;
	};
	
	class ast_exp_exp : public ast_exp
	{
		std::shared_ptr<ast_exp> exp;
	
	public:
		ast_exp_exp(__decltype(exp) exp)
		: exp(exp)
		{}
	
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s) const;
	};
	
	class ast_exp_fun_call : public ast_exp
	{
		std::shared_ptr<ast_fun_call> c;
	
	public:
		ast_exp_fun_call(__decltype(c) c)
		: c(c)
		{}
	
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s) const;
	};
	
	class ast_exp_nil : public ast_exp
	{
	public:
		ast_exp_nil()
		{}
		
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s) const;
	};
	
	class ast_exp_tuple : public ast_exp
	{
		std::shared_ptr<ast_exp> e_left, e_right;
	
	public:
		ast_exp_tuple(__decltype(e_left) e_left, __decltype(e_right) e_right)
		: e_left(e_left)
		, e_right(e_right)
		{}
		
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s) const;
	};
}

#endif

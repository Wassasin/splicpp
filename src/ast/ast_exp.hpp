#ifndef AST_EXP_H
#define AST_EXP_H

#include <boost/array.hpp>

#include "../common/typedefs.hpp"
#include "../typing/substitution.hpp"

#include "ast.hpp"

namespace splicpp
{
	class ast_id;
	class ast_fun_call;
	class symboltable;
	class varcontext;
	class sl_type;
	class typecontext;
	class ircontext;
	class ir_exp;

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
		
		ast_exp(const sloc sl)
		: ast(sl)
		{}
		
		virtual void assign_ids(const varcontext& c) = 0;
		
		virtual ast_exp_type type() const = 0;
		virtual void pretty_print(std::ostream& s, const uint tab) const = 0;
		virtual substitution infer_type(const typecontext& c, const s_ptr<const sl_type> t) const = 0;
		
		virtual s_ptr<const ir_exp> translate(const ircontext& c) const = 0;
	};
	
	class ast_exp_id : public ast_exp
	{
	public:
		const s_ptr<ast_id> id;
		
		ast_exp_id(__decltype(id) id, const sloc sl)
		: ast_exp(sl)
		, id(id)
		{}
	
		virtual void assign_ids(const varcontext& c);
	
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual substitution infer_type(const typecontext& c, const s_ptr<const sl_type> t) const;
		
		virtual s_ptr<const ir_exp> translate(const ircontext& c) const;
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
		const boost::array<const std::string, OP_TYPE_LENGTH> string_map;
		
	public:
		const s_ptr<ast_exp> e_left, e_right;
		const op_type t;
	
		ast_exp_op2(const __decltype(e_left) e_left, const op_type t, const __decltype(e_right) e_right, const sloc sl)
		: ast_exp(sl)
		, string_map({ { "+", "-", "*", "/", "%", "==", "<", ">", "<=", ">=", "!=", "&&", "||", ":" } })
		, e_left(e_left)
		, e_right(e_right)
		, t(t)
		{}
	
		op_type optype() const;
		
		virtual void assign_ids(const varcontext& c);
		
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual substitution infer_type(const typecontext& c, const s_ptr<const sl_type> t) const;
		
		virtual s_ptr<const ir_exp> translate(const ircontext& c) const;
	};
	
	class ast_exp_negation : public ast_exp
	{
	public:
		const s_ptr<ast_exp> exp;
	
		ast_exp_negation(__decltype(exp) exp, const sloc sl)
		: ast_exp(sl)
		, exp(exp)
		{}
	
		virtual void assign_ids(const varcontext& c);
	
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual substitution infer_type(const typecontext& c, const s_ptr<const sl_type> t) const;
		
		virtual s_ptr<const ir_exp> translate(const ircontext& c) const;
	};
	
	class ast_exp_int : public ast_exp
	{
	public:
		const int i;
	
		ast_exp_int(__decltype(i) i, const sloc sl)
		: ast_exp(sl)
		, i(i)
		{}
	
		virtual void assign_ids(const varcontext& c);
	
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual substitution infer_type(const typecontext& c, const s_ptr<const sl_type> t) const;
		
		virtual s_ptr<const ir_exp> translate(const ircontext& c) const;
	};
	
	class ast_exp_bool : public ast_exp
	{
	public:
		const bool b;
		
		ast_exp_bool(__decltype(b) b, const sloc sl)
		: ast_exp(sl)
		, b(b)
		{}
	
		virtual void assign_ids(const varcontext& c);
	
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual substitution infer_type(const typecontext& c, const s_ptr<const sl_type> t) const;
		
		virtual s_ptr<const ir_exp> translate(const ircontext& c) const;
	};
	
	class ast_exp_exp : public ast_exp
	{
	public:
		const s_ptr<ast_exp> exp;
		
		ast_exp_exp(__decltype(exp) exp, const sloc sl)
		: ast_exp(sl)
		, exp(exp)
		{}
	
		virtual void assign_ids(const varcontext& c);
	
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual substitution infer_type(const typecontext& c, const s_ptr<const sl_type> t) const;
		
		virtual s_ptr<const ir_exp> translate(const ircontext& c) const;
	};
	
	class ast_exp_fun_call : public ast_exp
	{
	public:
		const s_ptr<ast_fun_call> c;
	
		ast_exp_fun_call(__decltype(c) c, const sloc sl)
		: ast_exp(sl)
		, c(c)
		{}
	
		virtual void assign_ids(const varcontext& c);
	
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual substitution infer_type(const typecontext& c, const s_ptr<const sl_type> t) const;
		
		virtual s_ptr<const ir_exp> translate(const ircontext& c) const;
	};
	
	class ast_exp_nil : public ast_exp
	{
	public:
		ast_exp_nil(const sloc sl)
		: ast_exp(sl)
		{}

		virtual void assign_ids(const varcontext& c);

		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual substitution infer_type(const typecontext& c, const s_ptr<const sl_type> t) const;
		
		virtual s_ptr<const ir_exp> translate(const ircontext& c) const;
	};
	
	class ast_exp_tuple : public ast_exp
	{
	public:
		const s_ptr<ast_exp> e_left, e_right;
	
		ast_exp_tuple(decltype(e_left) e_left, decltype(e_right) e_right, const sloc sl)
		: ast_exp(sl)
		, e_left(e_left)
		, e_right(e_right)
		{}
		
		virtual void assign_ids(const varcontext& c);
		
		virtual ast_exp_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual substitution infer_type(const typecontext& c, const s_ptr<const sl_type> t) const;
		
		virtual s_ptr<const ir_exp> translate(const ircontext& c) const;
	};
}

#endif

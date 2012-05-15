#ifndef AST_EXP_MAPPER_H
#define AST_EXP_MAPPER_H

namespace splicpp
{
	class ast_exp_id;
	class ast_exp_op2;
	class ast_exp_negation;
	class ast_exp_int;
	class ast_exp_bool;
	class ast_exp_exp;
	class ast_exp_fun_call;
	class ast_exp_nil;
	class ast_exp_tuple;
	
	class ast_exp_mapper
	{
	public:
		virtual ~ast_exp_mapper() {}
		
		virtual void map(const s_ptr<const ast_exp_id> x) = 0;
		virtual void map(const s_ptr<const ast_exp_op2> x) = 0;
		virtual void map(const s_ptr<const ast_exp_negation> x) = 0;
		virtual void map(const s_ptr<const ast_exp_int> x) = 0;
		virtual void map(const s_ptr<const ast_exp_bool> x) = 0;
		virtual void map(const s_ptr<const ast_exp_exp> x) = 0;
		virtual void map(const s_ptr<const ast_exp_fun_call> x) = 0;
		virtual void map(const s_ptr<const ast_exp_nil> x) = 0;
		virtual void map(const s_ptr<const ast_exp_tuple> x) = 0;
	};
}

#endif

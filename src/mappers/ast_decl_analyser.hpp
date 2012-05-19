#ifndef AST_DECL_ANALYSER_H
#define AST_DECL_ANALYSER_H

#include "../common/typedefs.hpp"
#include "../common/dgraph.hpp"

#include "generic/ast_exp_mapper.hpp"
#include "generic/ast_stmt_mapper.hpp"

namespace splicpp
{
	class symboltable;

	class ast_decl;
	class ast_decl_fun;
	class ast_decl_var;
	
	class ast_exp;
	class ast_stmt;
	
	class ast_exp_id;
	class ast_exp_op2;
	class ast_exp_negation;
	class ast_exp_int;
	class ast_exp_bool;
	class ast_exp_exp;
	class ast_exp_fun_call;
	class ast_exp_nil;
	class ast_exp_tuple;
	
	class ast_stmt_stmts;
	class ast_stmt_if;
	class ast_stmt_while;
	class ast_stmt_assignment;
	class ast_stmt_fun_call;
	class ast_stmt_return;
	
	class ast_decl_analyser : public ast_exp_mapper, public ast_stmt_mapper
	{
		dgraph<size_t> g;
		sid i;
		
		ast_decl_analyser()
		: g()
		, i(0)
		{}
		
		void analyse(const s_ptr<const ast_decl_fun> x);
		void analyse(const s_ptr<const ast_decl_var> x);
		
		void analyse(const s_ptr<const ast_exp> x);
		void analyse(const s_ptr<const ast_stmt> x);
	public:
		static dgraph<sid> analyse(const std::vector<s_ptr<ast_decl>>& decls, const symboltable& t);
		
		virtual void map(const s_ptr<const ast_exp_id> x);
		virtual void map(const s_ptr<const ast_exp_op2> x);
		virtual void map(const s_ptr<const ast_exp_negation> x);
		virtual void map(const s_ptr<const ast_exp_int> x);
		virtual void map(const s_ptr<const ast_exp_bool> x);
		virtual void map(const s_ptr<const ast_exp_exp> x);
		virtual void map(const s_ptr<const ast_exp_fun_call> x);
		virtual void map(const s_ptr<const ast_exp_nil> x);
		virtual void map(const s_ptr<const ast_exp_tuple> x);
		
		virtual void map(const s_ptr<const ast_stmt_stmts> x);
		virtual void map(const s_ptr<const ast_stmt_if> x);
		virtual void map(const s_ptr<const ast_stmt_while> x);
		virtual void map(const s_ptr<const ast_stmt_assignment> x);
		virtual void map(const s_ptr<const ast_stmt_fun_call> x);
		virtual void map(const s_ptr<const ast_stmt_return> x);
	};
}

#endif

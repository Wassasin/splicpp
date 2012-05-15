#ifndef AST_STMT_MAPPER_H
#define AST_STMT_MAPPER_H

namespace splicpp
{
	class ast_stmt_stmts;
	class ast_stmt_if;
	class ast_stmt_while;
	class ast_stmt_assignment;
	class ast_stmt_fun_call;
	class ast_stmt_return;
	
	class ast_stmt_mapper
	{
	public:
		virtual ~ast_stmt_mapper() {}
		
		virtual void map(const s_ptr<const ast_stmt_stmts> x) = 0;
		virtual void map(const s_ptr<const ast_stmt_if> x) = 0;
		virtual void map(const s_ptr<const ast_stmt_while> x) = 0;
		virtual void map(const s_ptr<const ast_stmt_assignment> x) = 0;
		virtual void map(const s_ptr<const ast_stmt_fun_call> x) = 0;
		virtual void map(const s_ptr<const ast_stmt_return> x) = 0;
	};
}

#endif

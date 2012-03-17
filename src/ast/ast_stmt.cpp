#include "ast_stmt.hpp"

namespace splicpp
{
	/* ast_stmt_stmts */

	void ast_stmt_stmts::add_stmt(std::shared_ptr<ast_stmt> stmt)
	{
		stmts.push_back(stmt);
	}

	ast_stmt::ast_stmt_type ast_stmt_stmts::type() const
	{
		return type_stmts;
	}
	
	void ast_stmt_stmts::pretty_print(std::ostream& s, const uint tab) const
	{
		s << '{';
		for(const auto stmt : stmts)
		{
			ast::print_newline(s, tab+1);
			stmt->pretty_print(s, tab+1);
		}
		ast::print_newline(s, tab);
		s << '}';
	}
	
	/* ast_stmt_if */
	
	ast_stmt::ast_stmt_type ast_stmt_if::type() const
	{
		return type_if;
	}
	
	void ast_stmt_if::pretty_print(std::ostream& s, const uint tab) const
	{
		s << "if(";
		exp->pretty_print(s, tab);
		s << ')';
		
		ast::print_newline(s, tab);
		stmt_true->pretty_print(s,tab);
		
		if(stmt_false)
		{
			ast::print_newline(s, tab);
			s << "else";
			ast::print_newline(s, tab);
			stmt_false.get()->pretty_print(s, tab);
		}
	}
	
	/* ast_stmt_while */
	
	ast_stmt::ast_stmt_type ast_stmt_while::type() const
	{
		return type_while;
	}
	
	void ast_stmt_while::pretty_print(std::ostream& s, const uint tab) const
	{
		s << "while(";
		exp->pretty_print(s, tab);
		s << ')';
		ast::print_newline(s, tab);
		stmt->pretty_print(s, tab);
	}
	
	/* ast_stmt_assignment */
	
	ast_stmt::ast_stmt_type ast_stmt_assignment::type() const
	{
		return type_assignment;
	}
	
	void ast_stmt_assignment::pretty_print(std::ostream& s, const uint tab) const
	{
		id->pretty_print(s, tab);
		s << " = ";
		exp->pretty_print(s, tab);
		s << ';';
	}
	
	/* ast_stmt_fun_call */
	
	ast_stmt::ast_stmt_type ast_stmt_fun_call::type() const
	{
		return type_fun_call;
	}
	
	void ast_stmt_fun_call::pretty_print(std::ostream& s, const uint tab) const
	{
		f->pretty_print(s, tab);
		s << ';';
	}
	
	/* ast_stmt_return */
	
	ast_stmt::ast_stmt_type ast_stmt_return::type() const
	{
		return type_return;
	}
	
	void ast_stmt_return::pretty_print(std::ostream& s, const uint tab) const
	{
		s << "return ";
		exp->pretty_print(s, tab);
		s << ';';
	}
}
#ifndef AST_STMT_H
#define AST_STMT_H

#include <boost/optional.hpp>

#include "ast.hpp"

#include <memory>
#include <vector>

namespace splicpp
{
	class ast_id;
	class ast_exp;
	class ast_fun_call;
	class symboltable;
	class varcontext;

	class ast_stmt : public ast
	{
	public:
		enum ast_stmt_type
		{
			type_stmts,
			type_if,
			type_while,
			type_assignment,
			type_fun_call,
			type_return
		};
		
		ast_stmt(const sloc sl)
		: ast(sl)
		{}
		
		virtual void assign_ids(const varcontext& c) = 0;
		
		virtual ast_stmt_type type() const = 0;
		virtual void pretty_print(std::ostream& s, const uint tab) const = 0;
	};
	
	class ast_stmt_stmts : public ast_stmt
	{
		std::vector<std::shared_ptr<ast_stmt>> stmts;
	
	public:
		ast_stmt_stmts(const sloc sl)
		: ast_stmt(sl)
		, stmts()
		{}
	
		void add_stmt(std::shared_ptr<ast_stmt> stmt);
	
		virtual void assign_ids(const varcontext& c);
	
		virtual ast_stmt_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
	
	class ast_stmt_if : public ast_stmt
	{
		std::shared_ptr<ast_exp> exp;
		std::shared_ptr<ast_stmt> stmt_true;
		boost::optional<std::shared_ptr<ast_stmt>> stmt_false;
	
	public:
		ast_stmt_if(std::shared_ptr<ast_exp> exp, std::shared_ptr<ast_stmt> stmt_true, const sloc sl)
		: ast_stmt(sl)
		, exp(exp)
		, stmt_true(stmt_true)
		, stmt_false()
		{}
		
		ast_stmt_if(std::shared_ptr<ast_exp> exp, std::shared_ptr<ast_stmt> stmt_true, std::shared_ptr<ast_stmt> stmt_false, const sloc sl)
		: ast_stmt(sl)
		, exp(exp)
		, stmt_true(stmt_true)
		, stmt_false(stmt_false)
		{}
		
		virtual void assign_ids(const varcontext& c);
		
		virtual ast_stmt_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
	
	class ast_stmt_while : public ast_stmt
	{
		std::shared_ptr<ast_exp> exp;
		std::shared_ptr<ast_stmt> stmt;
	
	public:
		ast_stmt_while(__decltype(exp) exp, __decltype(stmt) stmt, const sloc sl)
		: ast_stmt(sl)
		, exp(exp)
		, stmt(stmt)
		{}
		
		virtual void assign_ids(const varcontext& c);
		
		virtual ast_stmt_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
	
	class ast_stmt_assignment : public ast_stmt
	{
		std::shared_ptr<ast_id> id;
		std::shared_ptr<ast_exp> exp;
		
	public:
		ast_stmt_assignment(__decltype(id) id, __decltype(exp) exp, const sloc sl)
		: ast_stmt(sl)
		, id(id)
		, exp(exp)
		{}
		
		virtual void assign_ids(const varcontext& c);
		
		virtual ast_stmt_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
	
	class ast_stmt_fun_call : public ast_stmt
	{
		std::shared_ptr<ast_fun_call> f;
		
	public:
		ast_stmt_fun_call(__decltype(f) f, const sloc sl)
		: ast_stmt(sl)
		, f(f)
		{}
		
		virtual void assign_ids(const varcontext& c);
		
		virtual ast_stmt_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;		
	};
	
	class ast_stmt_return : public ast_stmt
	{
		boost::optional<std::shared_ptr<ast_exp>> exp;
	
	public:
		ast_stmt_return(const sloc sl)
		: ast_stmt(sl)
		, exp()
		{}
	
		ast_stmt_return(__decltype(exp) exp, const sloc sl)
		: ast_stmt(sl)
		, exp(exp)
		{}
		
		virtual void assign_ids(const varcontext& c);
		
		virtual ast_stmt_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

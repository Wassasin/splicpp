#ifndef AST_STMT_H
#define AST_STMT_H

#include <boost/optional.hpp>

#include "ast.hpp"

#include "ast_id.hpp"
#include "ast_exp.hpp"
#include "ast_fun_call.hpp"

namespace splicpp
{
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
		
		virtual ast_stmt_type type() const = 0;
		virtual void pretty_print(std::ostream& s, const uint tab) const = 0;
	};
	
	class ast_stmt_stmts : public ast_stmt
	{
		std::vector<std::shared_ptr<ast_stmt>> stmts;
	
	public:
		ast_stmt_stmts()
		: stmts()
		{}
	
		void add_stmt(std::shared_ptr<ast_stmt> stmt);
	
		virtual ast_stmt_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
	
	class ast_stmt_if : public ast_stmt
	{
		std::shared_ptr<ast_exp> exp;
		std::shared_ptr<ast_stmt> stmt_true;
		boost::optional<std::shared_ptr<ast_stmt>> stmt_false;
	
	public:
		ast_stmt_if(std::shared_ptr<ast_exp> exp, std::shared_ptr<ast_stmt> stmt_true)
		: exp(exp)
		, stmt_true(stmt_true)
		, stmt_false()
		{}
		
		ast_stmt_if(std::shared_ptr<ast_exp> exp, std::shared_ptr<ast_stmt> stmt_true, std::shared_ptr<ast_stmt> stmt_false)
		: exp(exp)
		, stmt_true(stmt_true)
		, stmt_false(stmt_false)
		{}
		
		virtual ast_stmt_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
	
	class ast_stmt_while : public ast_stmt
	{
		std::shared_ptr<ast_exp> exp;
		std::shared_ptr<ast_stmt> stmt;
	
	public:
		ast_stmt_while(__decltype(exp) exp, __decltype(stmt) stmt)
		: exp(exp)
		, stmt(stmt)
		{}
		
		virtual ast_stmt_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
	
	class ast_stmt_assignment : public ast_stmt
	{
		std::shared_ptr<ast_id> id;
		std::shared_ptr<ast_exp> exp;
		
	public:
		ast_stmt_assignment(__decltype(id) id, __decltype(exp) exp)
		: id(id)
		, exp(exp)
		{}
		
		virtual ast_stmt_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
	
	class ast_stmt_fun_call : public ast_stmt
	{
		std::shared_ptr<ast_fun_call> f;
		
	public:
		ast_stmt_fun_call(__decltype(f) f)
		: f(f)
		{}
		
		virtual ast_stmt_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;		
	};
	
	class ast_stmt_return : public ast_stmt
	{
		std::shared_ptr<ast_exp> exp;
	
	public:
		ast_stmt_return(__decltype(exp) exp)
		: exp(exp)
		{}
		
		virtual ast_stmt_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

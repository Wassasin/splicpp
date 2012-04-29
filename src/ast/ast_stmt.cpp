#include "ast_stmt.hpp"

#include "ast_id.hpp"
#include "ast_exp.hpp"
#include "ast_fun_call.hpp"

#include "../typing/typecontext.hpp"
#include "../typing/types/sl_polytype.hpp"
#include "../typing/types/sl_type.hpp"
#include "../typing/types/sl_type_bool.hpp"
#include "../typing/types/sl_type_unbound.hpp"
#include "../typing/types/sl_type_void.hpp"

namespace splicpp
{
	/* ast_stmt_stmts */

	void ast_stmt_stmts::add_stmt(s_ptr<ast_stmt> stmt)
	{
		stmts.push_back(stmt);
	}
	
	void ast_stmt_stmts::assign_ids(const varcontext& c)
	{
		for(auto stmt : stmts)
			stmt->assign_ids(c);
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
	
	substitution ast_stmt_stmts::infer_type(const typecontext& c, const s_ptr<const sl_type> t) const
	{
		substitution s;
		for(const auto stmt : stmts)
			s = stmt->infer_type(c.apply(s), t->apply(s)).composite(s);
		
		return s;
	}
	
	bool ast_stmt_stmts::contains_return() const
	{
		for(const auto stmt : stmts)
			if(stmt->contains_return())
				return true;
	
		return false;
	}
	
	/* ast_stmt_if */
	
	void ast_stmt_if::assign_ids(const varcontext& c)
	{
		exp->assign_ids(c);
		stmt_true->assign_ids(c);
		if(stmt_false)
			stmt_false.get()->assign_ids(c);
	}
	
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
	
	substitution ast_stmt_if::infer_type(const typecontext& c, const s_ptr<const sl_type> t) const
	{
		substitution s = exp->infer_type(c, s_ptr<const sl_type>(new sl_type_bool(sl)));
		s = stmt_true->infer_type(c.apply(s), t->apply(s)).composite(s);
		
		if(stmt_false)
			s = stmt_false.get()->infer_type(c.apply(s), t->apply(s)).composite(s);
		
		return s;
	}
	
	bool ast_stmt_if::contains_return() const
	{
		return stmt_true->contains_return() || (stmt_false && stmt_false.get()->contains_return());
	}
	
	/* ast_stmt_while */
	
	void ast_stmt_while::assign_ids(const varcontext& c)
	{
		exp->assign_ids(c);
		stmt->assign_ids(c);
	}
	
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
	
	substitution ast_stmt_while::infer_type(const typecontext& c, const s_ptr<const sl_type> t) const
	{
		substitution s = exp->infer_type(c, s_ptr<const sl_type>(new sl_type_bool(sl)));
		return stmt->infer_type(c.apply(s), t->apply(s)).composite(s);
	}
	
	bool ast_stmt_while::contains_return() const
	{
		return stmt->contains_return();
	}
	
	/* ast_stmt_assignment */
	
	void ast_stmt_assignment::assign_ids(const varcontext& c)
	{
		id->assign_ids(c);
		exp->assign_ids(c);
	}
	
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
	
	substitution ast_stmt_assignment::infer_type(const typecontext& c, const s_ptr<const sl_type>) const
	{
		s_ptr<const sl_type> t;
		
		const s_ptr<const sl_polytype> dest = c[id->fetch_id()];
		const s_ptr<const sl_polytype_forall> dest_forall = std::dynamic_pointer_cast<const sl_polytype_forall>(dest);
		const s_ptr<const sl_polytype_exists> dest_exists = std::dynamic_pointer_cast<const sl_polytype_exists>(dest);
		
		if(dest_forall != nullptr) //Local var_decl or arg
			t = dest_forall->unbind_maintain();
		else if(dest_exists != nullptr) //Global var_decl
			t = dest_exists->unbind(c);
		else
			throw std::logic_error("Encountered unexpected type of sl_polytype, not sl_polytype_exists or sl_polytype_forall");
		
		return exp->infer_type(c, t);
	}
	
	bool ast_stmt_assignment::contains_return() const
	{
		return false;
	}
	
	/* ast_stmt_fun_call */
	
	void ast_stmt_fun_call::assign_ids(const varcontext& c)
	{
		f->assign_ids(c);
	}
	
	ast_stmt::ast_stmt_type ast_stmt_fun_call::type() const
	{
		return type_fun_call;
	}
	
	void ast_stmt_fun_call::pretty_print(std::ostream& s, const uint tab) const
	{
		f->pretty_print(s, tab);
		s << ';';
	}
	
	substitution ast_stmt_fun_call::infer_type(const typecontext& c, const s_ptr<const sl_type>) const
	{
		return f->infer_type(c, c.create_fresh(sl));
	}
	
	bool ast_stmt_fun_call::contains_return() const
	{
		return false;
	}
	
	/* ast_stmt_return */
	
	void ast_stmt_return::assign_ids(const varcontext& c)
	{
		if(exp)
			exp.get()->assign_ids(c);
	}
	
	ast_stmt::ast_stmt_type ast_stmt_return::type() const
	{
		return type_return;
	}
	
	void ast_stmt_return::pretty_print(std::ostream& s, const uint tab) const
	{
		s << "return";
		if(exp)
		{
			s << ' ';
			exp.get()->pretty_print(s, tab);
		}
		s << ';';
	}
	
	substitution ast_stmt_return::infer_type(const typecontext& c, const s_ptr<const sl_type> t) const
	{
		if(exp)
			return exp.get()->infer_type(c, t);
		else
			return t->unify(s_ptr<const sl_type>(new sl_type_void(sl)));
	}
	
	bool ast_stmt_return::contains_return() const
	{
		return true;
	}
}

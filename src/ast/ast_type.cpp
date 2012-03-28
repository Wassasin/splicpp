#include "ast_type.hpp"

#include "../typing/typecontext.hpp"
#include "../typing/types/sl_type_int.hpp"
#include "../typing/types/sl_type_bool.hpp"
#include "../typing/types/sl_type_tuple.hpp"
#include "../typing/types/sl_type_array.hpp"
#include "../typing/types/sl_type_void.hpp"

namespace splicpp
{
	void ast_type::register_type(const std::shared_ptr<ast_type> t, symboltable& s, varcontext& c)
	{
		if(t->type() == type_id)
		{
			auto tt = std::dynamic_pointer_cast<ast_type_id, ast_type>(t);
			const std::string name = tt->fetch_name();
			
			if(c.exists(name))
				tt->assign(c[name]);
			else
				c.assign(name, s.reg_type(tt));
		}
		else
			t->register_types(s, c);
	}

	/* ast_type_int */
	ast_type::ast_type_type ast_type_int::type() const
	{
		return type_int;
	}
	
	void ast_type_int::pretty_print(std::ostream& s, const uint) const
	{
		s << "Int";
	}
	
	std::shared_ptr<sl_type> ast_type_int::fetch_type(const typecontext&) const
	{
		return std::shared_ptr<sl_type>(new sl_type_int());
	}
	
	/* ast_type_bool */
	
	ast_type::ast_type_type ast_type_bool::type() const
	{
		return type_bool;
	}
	
	void ast_type_bool::pretty_print(std::ostream& s, const uint) const
	{
		s << "Bool";
	}
	
	std::shared_ptr<sl_type> ast_type_bool::fetch_type(const typecontext&) const
	{
		return std::shared_ptr<sl_type>(new sl_type_bool());
	}
	
	/* ast_type_tuple */
	
	ast_type::ast_type_type ast_type_tuple::type() const
	{
		return type_tuple;
	}
	
	void ast_type_tuple::register_types(symboltable& s, varcontext& c) const
	{
		ast_type::register_type(t_left, s, c);
		ast_type::register_type(t_right, s, c);
	}
	
	void ast_type_tuple::pretty_print(std::ostream& s, const uint tab) const
	{
		s << "(";
		t_left->pretty_print(s, tab);
		s << ", ";
		t_right->pretty_print(s, tab);
		s << ")";
	}
	
	std::shared_ptr<sl_type> ast_type_tuple::fetch_type(const typecontext& c) const
	{
		return std::shared_ptr<sl_type>(new sl_type_tuple(t_left->fetch_type(c), t_right->fetch_type(c)));
	}
	
	/* ast_type_array */
	
	ast_type::ast_type_type ast_type_array::type() const
	{
		return type_array;
	}
	
	void ast_type_array::register_types(symboltable& s, varcontext& c) const
	{
		ast_type::register_type(t, s, c);
	}
	
	void ast_type_array::pretty_print(std::ostream& s, const uint tab) const
	{
		s << "[";
		t->pretty_print(s, tab);
		s << "]";
	}
	
	std::shared_ptr<sl_type> ast_type_array::fetch_type(const typecontext& c) const
	{
		return std::shared_ptr<sl_type>(new sl_type_array(t->fetch_type(c)));
	}
	
	/* ast_type_id */
	
	void ast_type_id::assign(sid i)
	{
		id->assign(i);
	}
	
	std::string ast_type_id::fetch_name() const
	{
		return id->fetch_name();
	}
	
	ast_type::ast_type_type ast_type_id::type() const
	{
		return type_id;
	}
	
	void ast_type_id::pretty_print(std::ostream& s, const uint tab) const
	{
		id->pretty_print(s, tab);
	}
	
	std::shared_ptr<sl_type> ast_type_id::fetch_type(const typecontext& c) const
	{
		return c[id->fetch_id()];
	}
	
	/* ast_type_void */
	
	ast_type::ast_type_type ast_type_void::type() const
	{
		return type_void;
	}
	
	void ast_type_void::pretty_print(std::ostream& s, const uint) const
	{
		s << "Void";
	}
	
	std::shared_ptr<sl_type> ast_type_void::fetch_type(const typecontext&) const
	{
		return std::shared_ptr<sl_type>(new sl_type_void());
	}
}

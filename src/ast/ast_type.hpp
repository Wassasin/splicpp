#ifndef AST_TYPE_H
#define AST_TYPE_H

#include <tuple>
#include <memory>
#include <boost/optional.hpp>

#include "../common/typedefs.hpp"

#include "ast.hpp"
#include "ast_id.hpp"

#include "../typing/symboltable.hpp"
#include "../typing/varcontext.hpp"

namespace splicpp
{
	class ast_id;
	class ast_type_id;
	class symboltable;
	class varcontext;
	class sl_type;
	class typecontext;

	class ast_type : public ast
	{
	protected:
		virtual void register_types(symboltable&, varcontext&) const {}
	public:
		enum ast_type_type
		{
			type_int,
			type_bool,
			type_tuple,
			type_array,
			type_id
		};
		
		ast_type(const sloc sl)
		: ast(sl)
		{}
		
		virtual ast_type_type type() const = 0;
		virtual void pretty_print(std::ostream& s, const uint tab) const = 0;
		
		virtual std::shared_ptr<sl_type> fetch_type(const typecontext& c) const = 0;
		
		static void register_type(const std::shared_ptr<ast_type> t, symboltable& s, varcontext& c);
	};
	
	class ast_type_int : public ast_type
	{
	public:
		ast_type_int(const sloc sl)
		: ast_type(sl)
		{}
	
		virtual ast_type_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual std::shared_ptr<sl_type> fetch_type(const typecontext& c) const;
	};
	
	class ast_type_bool : public ast_type
	{
	public:
		ast_type_bool(const sloc sl)
		: ast_type(sl)
		{}
	
		virtual ast_type_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual std::shared_ptr<sl_type> fetch_type(const typecontext& c) const;
	};
	
	class ast_type_tuple : public ast_type
	{
		std::shared_ptr<ast_type> t_left, t_right;
		
	protected:
		virtual void register_types(symboltable& s, varcontext& c) const;
		
	public:
		ast_type_tuple(__decltype(t_left) t_left, __decltype(t_right) t_right, const sloc sl)
		: ast_type(sl)
		, t_left(t_left)
		, t_right(t_right)
		{}
	
		virtual ast_type_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual std::shared_ptr<sl_type> fetch_type(const typecontext& c) const;
	};
	
	class ast_type_array : public ast_type
	{
		std::shared_ptr<ast_type> t;
	protected:
		virtual void register_types(symboltable& s, varcontext& c) const;
		
	public:
		ast_type_array(__decltype(t) t, const sloc sl)
		: ast_type(sl)
		, t(t)
		{}
		
		virtual ast_type_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual std::shared_ptr<sl_type> fetch_type(const typecontext& c) const;
	};
	
	class ast_type_id : public ast_type
	{
		std::shared_ptr<ast_id> id;
		
	public:
		ast_type_id(__decltype(id) id, const sloc sl)
		: ast_type(sl)
		, id(id)
		{}
		
		void assign(sid i);
		std::string fetch_name() const;
		
		virtual ast_type_type type() const;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
		virtual std::shared_ptr<sl_type> fetch_type(const typecontext& c) const;
	};
}

#endif

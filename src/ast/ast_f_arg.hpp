#ifndef AST_F_ARG_H
#define AST_F_ARG_H

#include "../common/typedefs.hpp"

#include "ast.hpp"

#include "../typing/substitution.hpp"

namespace splicpp
{
	class ast_type;
	class ast_id;
	class symboltable;
	class varcontext;
	class sl_type;
	class typecontext;

	class ast_f_arg : public ast
	{
	public:	
		const s_ptr<ast_type> t;
		const s_ptr<ast_id> id;
		
		ast_f_arg(__decltype(t) t, __decltype(id) id, const sloc sl)
		: ast(sl)
		, t(t)
		, id(id)
		{}
		
		void assign(sid i);
		std::string fetch_name() const;
		sid fetch_id() const;
		
		void register_types(symboltable& s, varcontext& c);
		
		s_ptr<const sl_type> fetch_assigned_type(const typecontext& c) const;
		s_ptr<const sl_type_unbound> declare_type(typecontext& c) const;
		
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

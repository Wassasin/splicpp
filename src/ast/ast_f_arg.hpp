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
		std::shared_ptr<ast_type> t;
		std::shared_ptr<ast_id> id;

	public:	
		ast_f_arg(__decltype(t) t, __decltype(id) id, const sloc sl)
		: ast(sl)
		, t(t)
		, id(id)
		{}
		
		void assign(sid i);
		std::string fetch_name() const;
		
		void register_types(symboltable& s, varcontext& c);
		
		std::shared_ptr<sl_type> fetch_assigned_type(const typecontext& c) const;
		substitution infer_type(const typecontext& c, const std::shared_ptr<sl_type> t) const;
		
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
}

#endif

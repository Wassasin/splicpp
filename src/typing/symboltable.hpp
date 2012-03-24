#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <memory>
#include <vector>
#include <ostream>

#include "../common/typedefs.hpp"

#include "../ast/ast_fun_decl.hpp"
#include "../ast/ast_construct.hpp"
#include "../ast/ast_var_decl.hpp"
#include "../ast/ast_f_arg.hpp"
#include "../ast/ast_type.hpp"

namespace splicpp
{
	//Forward decls
	class ast_fun_decl;
	class ast_var_decl;
	class ast_f_arg;
	class ast_type_id;

	class symboltable
	{
		struct symbolref
		{
			enum symbolreftype
			{
				t_fun,
				t_construct,
				t_var,
				t_arg,
				t_local_var,
				t_type
			};
			
			symbolreftype t;
			size_t i;
			
			symbolref(symbolreftype t, size_t i)
			: t(t)
			, i(i)
			{}
		};
		
		std::vector<symbolref> index;
		std::vector<std::shared_ptr<ast_fun_decl>> funs;
		std::vector<std::shared_ptr<ast_construct>> conss;
		std::vector<std::shared_ptr<ast_var_decl>> vars;
		std::vector<std::shared_ptr<ast_f_arg>> args;
		std::vector<std::shared_ptr<ast_var_decl>> local_vars;
		std::vector<std::shared_ptr<ast_type_id>> types;
	
		sid create_entry(symbolref::symbolreftype t, size_t i);
	
	public:
		symboltable()
		: index()
		, funs()
		, conss()
		, vars()
		, args()
		, local_vars()
		, types()
		{}
		
		sid reg_fun(std::shared_ptr<ast_fun_decl> f);
		sid reg_cons(std::shared_ptr<ast_construct> c);
		sid reg_var(std::shared_ptr<ast_var_decl> v);
		sid reg_arg(std::shared_ptr<ast_f_arg> a);
		sid reg_lvar(std::shared_ptr<ast_var_decl> lv);
		sid reg_type(std::shared_ptr<ast_type_id> t);
		
		void print(std::ostream& s) const;
	};
}

#endif

#ifndef AST_CONSTRUCT_H
#define AST_CONSTRUCT_H

#include "ast.hpp"

namespace splicpp
{
	class ast_construct : public ast
	{
	public:
		ast_construct()
		: ast(sloc(0, 0)) //Bogus sLoc
		{}
		
		virtual std::string fetch_name() const = 0;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
	
	/* print, isEmpty, head, tail, fst, and snd */
	
	class ast_construct_print : public ast_construct
	{
	public:
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_is_empty : public ast_construct
	{
	public:
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_head : public ast_construct
	{
	public:
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_tail : public ast_construct
	{
	public:
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_fst : public ast_construct
	{
	public:
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_snd : public ast_construct
	{
	public:
		virtual std::string fetch_name() const;
	};
}

#endif

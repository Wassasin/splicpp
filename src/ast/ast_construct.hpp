#ifndef AST_CONSTRUCT_H
#define AST_CONSTRUCT_H

#include <memory>
#include <boost/optional.hpp>

#include "ast.hpp"
#include "../common/typedefs.hpp"
#include "../typing/substitution.hpp"

namespace splicpp
{
	class sl_type;
	class sl_polytype;
	class typecontext;

	class ast_construct : public ast
	{
		boost::optional<sid> id;
	
	protected:
		virtual s_ptr<sl_type> fetch_type(const typecontext& c) const = 0;
	public:
		ast_construct()
		: ast(sloc(0, 0)) //Bogus sLoc
		, id()
		{}
		
		void assign(const sid i);
		substitution declare_type(typecontext& c) const;
		
		virtual std::string fetch_name() const = 0;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
	
	/* print, isEmpty, head, tail, fst, and snd */
	
	class ast_construct_print : public ast_construct
	{
	protected:
		virtual s_ptr<sl_type> fetch_type(const typecontext& c) const;
	public:
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_is_empty : public ast_construct
	{
	protected:
		virtual s_ptr<sl_type> fetch_type(const typecontext& c) const;
	public:
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_head : public ast_construct
	{
	protected:
		virtual s_ptr<sl_type> fetch_type(const typecontext& c) const;
	public:
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_tail : public ast_construct
	{
	protected:
		virtual s_ptr<sl_type> fetch_type(const typecontext& c) const;
	public:
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_fst : public ast_construct
	{
	protected:
		virtual s_ptr<sl_type> fetch_type(const typecontext& c) const;
	public:
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_snd : public ast_construct
	{
	protected:
		virtual s_ptr<sl_type> fetch_type(const typecontext& c) const;
	public:
		virtual std::string fetch_name() const;
	};
}

#endif

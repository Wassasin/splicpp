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
	class ircontext;
	class ir_stmt;

	class ast_construct : public ast
	{
	protected:
		boost::optional<sid> id;
		
		virtual s_ptr<const sl_type> fetch_type(const typecontext& c) const = 0;
	public:
		ast_construct()
		: ast(sloc()) //Bogus sLoc
		, id()
		{}
		
		void assign(const sid i);
		substitution declare_type(typecontext& c) const;
		
		virtual s_ptr<const ir_stmt> translate(const ir_label l_construct, const ircontext& c) const = 0;
		virtual std::string fetch_name() const = 0;
		virtual void pretty_print(std::ostream& s, const uint tab) const;
	};
	
	/* print, isEmpty, head, tail, fst, and snd */
	
	class ast_construct_print : public ast_construct
	{
	protected:
		virtual s_ptr<const sl_type> fetch_type(const typecontext& c) const;
	public:
		virtual s_ptr<const ir_stmt> translate(const ir_label l_construct, const ircontext& c) const;
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_is_empty : public ast_construct
	{
	protected:
		virtual s_ptr<const sl_type> fetch_type(const typecontext& c) const;
	public:
		virtual s_ptr<const ir_stmt> translate(const ir_label l_construct, const ircontext& c) const;
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_head : public ast_construct
	{
	protected:
		virtual s_ptr<const sl_type> fetch_type(const typecontext& c) const;
	public:
		virtual s_ptr<const ir_stmt> translate(const ir_label l_construct, const ircontext& c) const;
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_tail : public ast_construct
	{
	protected:
		virtual s_ptr<const sl_type> fetch_type(const typecontext& c) const;
	public:
		virtual s_ptr<const ir_stmt> translate(const ir_label l_construct, const ircontext& c) const;
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_fst : public ast_construct
	{
	protected:
		virtual s_ptr<const sl_type> fetch_type(const typecontext& c) const;
	public:
		virtual s_ptr<const ir_stmt> translate(const ir_label l_construct, const ircontext& c) const;
		virtual std::string fetch_name() const;
	};
	
	class ast_construct_snd : public ast_construct
	{
	protected:
		virtual s_ptr<const sl_type> fetch_type(const typecontext& c) const;
	public:
		virtual s_ptr<const ir_stmt> translate(const ir_label l_construct, const ircontext& c) const;
		virtual std::string fetch_name() const;
	};
}

#endif

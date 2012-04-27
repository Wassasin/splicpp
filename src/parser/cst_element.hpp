#ifndef CST_ELEMENT_H
#define CST_ELEMENT_H

#include <boost/optional.hpp>

#include "cst_node.hpp"

#include "../common/sloc.hpp"
#include "../common/token.hpp"
#include "../common/grammar.hpp"

namespace splicpp
{
	struct cst_node;

	struct cst_element
	{
		enum cst_element_type
		{
			t_token,
			t_node
		};
	
		cst_element_type type;
		
		boost::optional<token> tok;
		boost::optional<cst_node> node;
		
		cst_element(const token tok)
		: type(t_token)
		, tok(tok)
		, node()
		{}
		
		cst_element(const cst_node node)
		: type(t_node)
		, tok()
		, node(node)
		{}
		
		bool is_token() const;
		bool is_node() const;
		bool is_token(const grammar& g, const std::string str) const;
		bool is_node(const grammar& g, const std::string str) const;
		
		token as_token() const;
		cst_node as_node() const;
		
		sloc sl() const;
		void print(const grammar& g, const std::string source, const uint tab) const;
		
		void assert_token(const grammar& g, const std::string str) const;
		void assert_node(const grammar& g, const std::string str) const;
	};
}

#endif

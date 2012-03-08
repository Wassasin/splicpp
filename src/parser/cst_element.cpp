#include "cst_element.hpp"
#include "cst_node.hpp"

namespace splicpp
{
	bool cst_element::is_token() const
	{
		return(type == t_token);
	}
	
	bool cst_element::is_node() const
	{
		return(type == t_node);
	}
	
	bool cst_element::is_token(const grammar& g, const std::string str) const
	{
		if(!is_token())
			return false;
		
		return(g.fetch_symbol(as_token().type)->name == str);
	}
	
	bool cst_element::is_node(const grammar& g, const std::string str) const
	{
		if(!is_node())
			return false;
		
		return(g.fetch_symbol(as_node().fetch_stid(g))->name == str);
	}
	
	token cst_element::as_token() const
	{
		return tok.get();
	}
	
	cst_node cst_element::as_node() const
	{
		return node.get();
	}
	
	void cst_element::print(const grammar& g, const std::string source, const uint tab) const
	{
		if(type == t_token)
		{
			cst_node::print_tab(tab);
			g.print_token(as_token(), source);
		}
		else
			node.get().print(g, source, tab);
	}
	
	void cst_element::assert_token(const grammar& g, const std::string str) const
	{
		assert(is_token(g, str));
	}
	
	void cst_element::assert_node(const grammar& g, const std::string str) const
	{
		assert(is_node(g, str));
	}
}

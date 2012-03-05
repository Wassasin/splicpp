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
	
	token cst_element::as_token() const
	{
		return tok.get();
	}
	
	cst_node cst_element::as_node() const
	{
		return node.get();
	}
	
	void cst_element::print(const grammar g, const std::string source, const uint tab) const
	{
		if(type == t_token)
		{
			cst_node::print_tab(tab);
			g.print_token(as_token(), source);
		}
		else
			node.get().print(g, source, tab);
	}
	
	void cst_element::assert_token(const grammar g, const std::string str)
	{
		assert(is_token());
		assert(g.fetch_symbol(as_token().type)->name == str);
	}
}

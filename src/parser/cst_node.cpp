#include "cst_node.hpp"
#include "cst_element.hpp"

namespace splicpp
{
	void cst_node::add_element(const s_ptr<cst_element> x)
	{
		elements.push_front(x);
	}
	
	bool cst_node::is_full(const grammar& g) const
	{
		return(g.fetch_rule(r).body.size() == elements.size());
	}

	stid cst_node::fetch_stid(const grammar& g) const
	{
		return g.fetch_rule(r).start;
	}

	void cst_node::print(const grammar& g, const std::string source, const uint tab) const
	{
		print_tab(tab);
		std::cout << g.fetch_symbol(fetch_stid(g))->name << std::endl;
	
		BOOST_FOREACH(const s_ptr<cst_element> x, elements)
			x->print(g, source, tab+1);
	}

	s_ptr<cst_element> cst_node::operator[](size_t i) const
	{
		return elements.at(i);
	}

	void cst_node::assert_stid(const grammar& g, const stid i) const
	{
		assert(fetch_stid(g) == i);
	}

	void cst_node::assert_stid(const grammar& g, const std::string str) const
	{
		assert_stid(g, g.fetch_stid(str));
	}

	size_t cst_node::size() const
	{
		return elements.size();
	}
	
	sloc cst_node::sl() const
	{
		if(size() == 0)
			throw std::logic_error("Cannot provide sLoc of lambda-expression");
		
		return elements.front()->sl().until(elements.back()->sl());
	}
}

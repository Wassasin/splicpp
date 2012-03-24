#ifndef CST_NODE_H
#define CST_NODE_H

#include <deque>
#include <memory>
#include <boost/foreach.hpp>

#include "../common/grammar.hpp"
#include "../common/sloc.hpp"

namespace splicpp
{
	struct cst_element;

	struct cst_node
	{
		rid r;
		std::deque<std::shared_ptr<cst_element>> elements;
	
		cst_node(rid r)
		: r(r)
		, elements()
		{}
	
		void add_element(const std::shared_ptr<cst_element> x);
		bool is_full(const grammar& g) const;
		stid fetch_stid(const grammar& g) const;
		void print(const grammar& g, const std::string source, const uint tab) const;
		std::shared_ptr<cst_element> operator[](size_t i) const;
		void assert_stid(const grammar& g, const stid i) const;
		void assert_stid(const grammar& g, const std::string str) const;
		size_t size() const;
		sloc sl() const;

		static void print_tab(const uint tab)
		{
			for(uint i = 0; i < tab; i++)
				std::cout << '\t';
		}
	};
}

#endif

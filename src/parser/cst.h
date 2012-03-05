#ifndef CST_H
#define CST_H

#include <vector>
#include <memory>
#include <boost/optional.hpp>
#include <boost/foreach.hpp>

#include "../common/token.h"

namespace splicpp
{
	struct cst_node
	{
		enum cst_element_type
		{
			t_token,
			t_node
		};
	
		struct cst_element
		{
			cst_element_type type;
			
			boost::optional<token> tok;
			boost::optional<std::shared_ptr<cst_node>> node;
			
			cst_element(const token tok)
			: type(t_token)
			, tok(tok)
			, node()
			{}
			
			cst_element(const std::shared_ptr<cst_node> node)
			: type(t_node)
			, tok()
			, node(node)
			{}
			
			cst_element(const cst_element& x)
			: type(x.type)
			, tok()
			, node()
			{
				if(x.type == t_token)
					tok = x.tok;
				else if(x.type == t_node) 
					node = x.node;
			}
			
			bool is_token() const
			{
				return(type == t_token);
			}
			
			bool is_node() const
			{
				return(type == t_node);
			}
			
			token as_token() const
			{
				return tok.get();
			}
			
			std::shared_ptr<cst_node> as_node() const
			{
				return node.get();
			}
			
			void print(const grammar g, const std::string source, const uint tab) const
			{
				if(type == t_token)
				{
					print_tab(tab);
					g.print_token(as_token(), source);
				}
				else
					node.get()->print(g, source, tab);
			}
		};
	
		rid r;
		std::list<cst_element> elements;
		
		cst_node(rid r)
		: r(r)
		, elements()
		{}
		
		void add_element(const cst_element x)
		{
			elements.push_front(x);
		}
		
		bool is_full(const grammar g) const
		{
			return(g.fetch_rule(r).body.size() == elements.size());
		}
		
		stid fetch_stid(const grammar g) const
		{
			return g.fetch_rule(r).start;
		}
		
		void print(const grammar g, const std::string source, const uint tab) const
		{
			print_tab(tab);
			std::cout << g.fetch_symbol(fetch_stid(g))->name << std::endl;
			
			BOOST_FOREACH(const cst_element x, elements)
				x.print(g, source, tab+1);
		}
		
		static void print_tab(const uint tab)
		{
			for(uint i = 0; i < tab; i++)
				std::cout << '\t';
		}
	};
}

#endif

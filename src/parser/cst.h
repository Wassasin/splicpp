#ifndef CST_H
#define CST_H

#include <vector>
#include <memory>
#include <boost/optional.hpp>

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
			
			cst_element(token tok)
			: type(t_token)
			, tok(tok)
			, node()
			{}
			
			cst_element(std::shared_ptr<cst_node> node)
			: type(t_node)
			, tok()
			, node(node)
			{}
			
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
		};
	
		rid r;
		std::vector<cst_element> elements;
		
		cst_node(rid r)
		: r(r)
		, elements()
		{}
		
		void add_element(const cst_element x)
		{
			elements.push_back(x);
		}
		
		bool is_full(const grammar g) const
		{
			return(g.fetch_rule(r).body.size() == elements.size());
		}
		
		stid fetch_stid(const grammar g) const
		{
			return g.fetch_rule(r).start;
		}
	};
}

#endif

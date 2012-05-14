#ifndef CFGRAPH_H
#define CFGRAPH_H

#include <vector>
#include <queue>

#include "generic.hpp"

namespace splicpp
{	
	class cfgraph
	{
	public:
		typedef size_t node;
		
	private:
		std::vector<std::vector<node>> data;
		
	public:
		cfgraph(const size_t size)
		: data(size)
		{}
		
		void add_edge(const node from, const node to)
		{
			if(!is_in<node>(to, data.at(from)))
				data.at(from).push_back(to);
		}
		
		std::vector<node> succ(const node n) const
		{
			std::queue<node> q;
			std::vector<node> result;
			
			q.push(n);
			
			while(!q.empty())
			{
				const node t = q.front();
				q.pop();
				
				for(const node c : data.at(t))
				{
					if(is_in<node>(t, result))
						continue;
					
					result.push_back(c);
					q.push(c);
				}
			}
			
			return result;
		}
	};
}

#endif

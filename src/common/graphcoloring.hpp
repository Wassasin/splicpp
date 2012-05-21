#ifndef GRAPHCOLORING_H
#define GRAPHCOLORING_H

#include <map>
#include <vector>

#include "generic.hpp"
#include "udgraph.hpp"

namespace splicpp
{
	template<typename T>
	class graphcoloring
	{
	public:
		struct selected
		{
			std::map<T, size_t> map;
			std::vector<T> spilled;
			
			selected()
			: map()
			, spilled()
			{}
		};
		
	private:
		typedef std::vector<std::pair<T, bool>> stack;
	
		static stack simplify(udgraph<T> g, const size_t k)
		{
			stack s;
		
			std::vector<T> vertices = g.fetch_vertices();
			while(!vertices.empty())
			{
				bool added = false;
				for(const T x : vertices)
					if(g.edges_count(x) < k)
					{
						s.push_back(std::make_pair(x, false));
						g.remove_vertex(x);
						added = true;
						break;
					}
			
				if(!added) //No t with edges_count < k, spill
				{
					const T x = *vertices.cbegin();
				
					s.push_back(std::make_pair(x, true));
					g.remove_vertex(x);
				}
			
				vertices = g.fetch_vertices();
			}
		
			return s;
		}
		
		static boost::optional<selected> select(const selected& result, const stack& s, const size_t i, const udgraph<T>& g, const size_t k)
		{
			if(i >= s.size())
				return result; //Success!
	
			const std::pair<T, bool> current = s.at(i);
		
			if(current.second) //Spill
			{
				selected new_result = result;
				new_result.spilled.push_back(current.first);
				
				return select(new_result, s, i+1, g, k);
			}
			else
			{
				std::unordered_set<size_t> mapped_edges;
				for(const T x : g.succ(current.first))
				{
					const auto i = result.map.find(x);
					if(i == result.map.end())
						continue;
				
					mapped_edges.insert((*i).second);
				}
		
				for(size_t x = 0; x < k; x++)
				{
					if(is_in<size_t>(x, mapped_edges))
						continue; //Specific color is already in use
					
					//Can select color, try to continue
					selected new_result = result;
					new_result.map[current.first] = x;
				
					const boost::optional<selected> opt = select(new_result, s, i+1, g, k);
					if(opt) //Success!
						return opt;
				}
				
				return boost::optional<selected>(); //The selected result passed to this function is not colorable
			}
		}
		
	public:
		static selected color(const udgraph<T>& g, const size_t k)
		{
			return select(selected(), simplify(g, k), 0, g, k).get();
		}
	};
}

#endif

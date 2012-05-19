#ifndef CFGRAPH_H
#define CFGRAPH_H

#include <map>
#include <vector>
#include <unordered_set>
#include <queue>

#include "generic.hpp"

namespace splicpp
{
	template<typename T>
	class dgraph
	{
	private:
		std::map<T, size_t> vertices;
		std::vector<std::unordered_set<size_t>> out_edges;
		std::vector<std::unordered_set<size_t>> in_edges;
		
		size_t capacity;
		std::queue<size_t> empty_spots;
	
	public:
		dgraph()
		: vertices()
		, out_edges()
		, in_edges()
		, capacity()
		, empty_spots()
		{}
		
		void add_vertex(const T x)
		{
			size_t i;
			if(empty_spots.empty())
			{
				i = capacity++;
				out_edges.push_back(std::unordered_set<size_t>());
				in_edges.push_back(std::unordered_set<size_t>());
			}
			else
			{
				i = empty_spots.front();
				empty_spots.pop();
			}
			
			vertices[x] = i;
		}
		
		void add_edge(const T from, const T to)
		{
			const size_t i = vertices.at(from);
			const size_t j = vertices.at(to);
			
			out_edges.at(i).insert(j);
			in_edges.at(j).insert(i);
		}
		
		void remove_vertex(const T x)
		{
			const size_t i = vertices.at(x);
			
			vertices.erase(x);
			empty_spots.push(i);
			
			out_edges.at(i).clear();
			in_edges.at(i).clear();
			
			for(auto& edges : out_edges)
				edges.erase(i);
			
			for(auto& edges : in_edges)
				edges.erase(i);
		}
		
		size_t in_edges_count(const T x) const
		{
			const size_t i = vertices.at(x);
			return in_edges.at(i).size();
		}
		
		size_t out_edges_count(const T x) const
		{
			const size_t i = vertices.at(x);
			return out_edges.at(i).size();
		}
		
		std::unordered_set<const T> leaves() const
		{
			std::unordered_set<const T> result;
			
			for(const std::pair<const T, const size_t> p : vertices)
				if(out_edges.at(p.second).size == 0)
					result.push_back(p.first);
			
			return result;
		}
		
		/*std::vector<node> succ(const node n) const
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
		}*/
	};
}

#endif

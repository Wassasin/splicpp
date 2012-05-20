#ifndef UDGRAPH_H
#define UDGRAPH_H

#include <map>
#include <vector>
#include <unordered_set>
#include <queue>

#include "generic.hpp"

namespace splicpp
{
	template<typename T>
	class udgraph
	{
	private:
		std::map<T, size_t> vertices;
		std::vector<std::unordered_set<size_t>> edges;
		
		size_t capacity;
		std::queue<size_t> empty_spots;
	
		void add_edge_internal(const size_t from, const size_t to)
		{
			edges.at(from).insert(to);
			edges.at(to).insert(from);
		}

	public:
		udgraph()
		: vertices()
		, edges()
		, capacity()
		, empty_spots()
		{}
		
		void add_vertex(const T x)
		{
			if(vertices.find(x) != vertices.end()) //Map already contains element
				return;
		
			size_t i;
			if(empty_spots.empty())
			{
				i = capacity++;
				edges.push_back(std::unordered_set<size_t>());
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
			
			add_edge_internal(i, j);
		}
		
		void remove_vertex(const T x)
		{
			const size_t i = vertices.at(x);
		
			vertices.erase(x);
			empty_spots.push(i);
			
			edges.at(i).clear();
			
			for(auto& e : edges)
				e.erase(i);
		}
		
		size_t edges_count(const T x) const
		{
			const size_t i = vertices.at(x);
			return edges.at(i).size();
		}
	};
}

#endif

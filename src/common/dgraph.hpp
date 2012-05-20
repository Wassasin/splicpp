#ifndef DGRAPH_H
#define DGRAPH_H

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
	
		void add_edge_internal(const size_t from, const size_t to)
		{
			out_edges.at(from).insert(to);
			in_edges.at(to).insert(from);
		}

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
			if(vertices.find(x) != vertices.end()) //Map already contains element
				return;
		
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
			
			add_edge_internal(i, j);
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
		
		void remove_vertex_maintain_paths(const T x)
		{
			const size_t i = vertices.at(x);
			
			for(const size_t from : in_edges.at(i))
				for(const size_t to : out_edges.at(i))
					add_edge_internal(from, to);
			
			remove_vertex(x);
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
		
		std::unordered_set<T> leaves() const
		{
			std::unordered_set<T> result;
			
			for(const std::pair<const T, const size_t> p : vertices)
				if(out_edges.at(p.second).size() == 0)
					result.insert(p.first);
			
			return result;
		}
		
		std::unordered_set<T> pred(const T x) const
		{
			return in_edges.at(vertices.at(x));
		}
		
		std::unordered_set<T> succ(const T x) const
		{
			return out_edges.at(vertices.at(x));
		}
	};
}

#endif

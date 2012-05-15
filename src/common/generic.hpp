#ifndef GENERIC_H
#define GENERIC_H

#include <vector>
#include <boost/optional.hpp>

namespace splicpp
{
	template <typename T>
	static bool is_in(const T x, const std::vector<T>& xs)
	{
		for(size_t i = 0; i < xs.size(); i++)
			if(xs[i] == x)
				return true;
		
		return false;
	}
	
	template <typename T>
	static bool is_in_ptr(const s_ptr<T> x, const std::vector<s_ptr<T>>& xs)
	{
		for(size_t i = 0; i < xs.size(); i++)
			if(xs[i]->operator==(x))
				return true;
		
		return false;
	}
	
	template <typename T>
	static bool any_is_in_ptr(const std::vector<s_ptr<T>> xs, const std::vector<s_ptr<T>>& ys)
	{
		for(s_ptr<T> x : xs)
			if(is_in_ptr<T>(x, ys))
				return true;
		
		return false;
	}
	
	template <typename T>
	static void remove_duplicates(std::vector<T>& xs)
	{
		std::vector<T> tmp;
		
		for(size_t i = 0; i < xs.size(); i++)
			if(!is_in<T>(xs[i], tmp))
				tmp.push_back(xs[i]);
		
		xs.swap(tmp);
	}
	
	template <typename T>
	static void add_to(const std::vector<T> xs, std::vector<T>& ys)
	{
		for(size_t i = 0; i < xs.size(); i++)
			ys.push_back(xs[i]);
	}
	
	template <typename T>
	static std::vector<T> subvector(const std::vector<T> xs, const size_t start, size_t len)
	{
		if(len > xs.size() - start)
			len = xs.size() - start;
		
		std::vector<T> result(len);
		for(size_t i = 0; i < len; i++)
			result[i] = xs[start + i];
			
		return result;
	}
	
	template <typename T>
	static std::vector<T> subvector(const std::vector<T> xs, const size_t start)
	{
		return subvector<T>(xs, start, xs.size() - start);
	}
	
	template <typename T>
	static std::vector<s_ptr<T>> subtract_ptr(const std::vector<s_ptr<T>> xs, std::vector<s_ptr<T>> ys)
	{
		std::vector<s_ptr<T>> result;
		for(const auto& x : xs)
			if(!is_in_ptr<T>(x, ys))
				result.push_back(x);
		
		return result;
	}
	
	template <typename T>
	static std::vector<T> create_vector(const T x)
	{
		std::vector<T> xs;
		xs.push_back(x);
		return xs;
	}
	
	template <typename T>
	static std::vector<s_ptr<T>> create_vector_ptr(const s_ptr<T> x)
	{
		return create_vector<s_ptr<T>>(x);
	}
	
	template <typename T>
	static boost::optional<size_t> find(const T x, const std::vector<T>& xs)
	{
		for(size_t i = 0; i < xs.size(); i++)
			if(xs[i] == x)
				return i;
		
		return boost::optional<size_t>();
	}
}

#endif

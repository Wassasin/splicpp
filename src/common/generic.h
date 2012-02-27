#ifndef GENERIC_H
#define GENERIC_H

namespace splicpp
{
	template <typename T>
	static bool is_in(const T x, const std::vector<T> xs)
	{
		for(size_t i = 0; i < xs.size(); i++)
			if(xs[i] == x)
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
}

#endif

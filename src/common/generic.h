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
}

#endif

#ifndef ITEMSET_H
#define ITEMSET_H

#include <vector>

#include "item.h"

namespace splicpp
{
	template <size_t L>
	class itemset
	{
		std::vector<item<L>> contents;

	public:
		itemset()
		: contents()
		{}

		bool operator==(const itemset<L> i_set) const
		{
			if(i_set.size() != this->size())
				return false;
			
			for(size_t i = 0; i < i_set.size(); i++)
				if(!this->contains(i_set[i]))
					return false;

			return true;
		}

		bool operator!=(const itemset<L>& y) const
		{
			return(!((*this)==y));
		}

		bool contains(item<L> x) const
		{
			for(size_t i = 0; i < contents.size(); i++)
				if(contents.at(i) == x)
					return true;

			return false;
		}

		bool is_in(const std::vector<itemset<L>> c)
		{
			/*
			//TODO REMOVE
			std::cout << "----------" << std::endl << "item_is_in(1)" << std::endl;
			this->print(g);
			std::cout << std::endl;
			std::cout << "----------" << std::endl << "item_is_in(2)" << std::endl;
			print(c, g);
			std::cout << std::endl;
			//END TODO REMOVE
			*/
			
			for(size_t i = 0; i < c.size(); i++)
				if((*this) == c.at(i))
					return true;

			return false;
		}
		
		inline void push_back(const item<L> x)
		{
			contents.push_back(x);
		}
		
		inline item<L>& operator[](const size_t i)
		{
			return contents.at(i);
		}
		
		inline const item<L>& operator[](const size_t i) const
		{
			return contents.at(i);
		}
		
		inline size_t size() const
		{
			return contents.size();
		}

		void print(const grammar g) const
		{
			std::cout << '{';
			for(size_t i = 0; i < contents.size(); i++)
			{
				contents.at(i).print(g);
				std::cout << ", ";
			}
			std::cout << '}';
		}

		static void print(const std::vector<itemset<L>> c, const grammar g)
		{
			for(size_t i = 0; i < c.size(); i++)
			{
				c.at(i).print(g);
				std::cout << std::endl << std::endl;
			}
		}
	};
}

#endif



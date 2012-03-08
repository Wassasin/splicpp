#include "rule.hpp"

#include <iostream>

namespace splicpp
{
	void rule::append_body(const stid s)
	{
		body.push_back(s);
	}
	
	rule rule::operator+(stid s)
	{
		append_body(s);
		return *this;
	}
	
	bool rule::operator==(const rule x) const
	{
		if(start != x.start)
			return false;
		
		if(body.size() != x.body.size())
			return false;
		
		for(size_t i = 0; i < body.size(); i++)
			if(body[i] != x.body[i])
				return false;
		
		return true;
	}
	
	bool rule::operator!=(const rule x) const
	{
		return(!((*this) == x));
	}
	
	bool rule::is_epsilon() const
	{
		return body.empty();
	}
	
	void rule::print() const
	{
		std::cout << start << " :== ";
			
		for(size_t i = 0; i < body.size(); i++)
		{
			if(i > 0)
				std::cout << ' ';
				
			std::cout << body[i];
		}
		
		std::cout << std::endl;
	}
}

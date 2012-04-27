#include "sloc.hpp"

namespace splicpp
{
	void sloc::print(std::string str, std::ostream& s) const
	{
		size_t line_start;
		for(size_t i = 0; i < pos; i++)
			if(str[i] == '\n')
				line_start = i;
		
		for(size_t i = line_start+1; i < str.length(); i++)
		{
			if(str[i] == '\n')
				break;
			
			s << str[i];
		}
		
		s << std::endl;
		
		for(size_t i = line_start+1; i < pos; i++)
		{	
			if(str[i] == '\t')
				s << '\t';
			else
				s << ' ';
		}
		
		s << '^';
		
		for(size_t j = pos+1; j < pos+length; j++)
			s << '~';
	}
}

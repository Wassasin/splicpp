#include "sloc.hpp"

namespace splicpp
{
	sloc sloc::until(const sloc end) const
	{
		sloc result = *this;
		result.length = end.pos + end.length - pos;
		
		return result;
	}

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
		
		for(size_t i = pos+1; i < pos+length; i++)
		{
			if(str[i] == '\n')
				break;
			
			s << '~';
		}
	}
}

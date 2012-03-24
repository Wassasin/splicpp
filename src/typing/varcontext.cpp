#include "varcontext.hpp"

namespace splicpp
{
	bool varcontext::exists(const std::string str) const
	{
		return(c.find(str) != c.end());
	}
	
	void varcontext::assign(const std::string str, const sid i)
	{
		c[str] = i;
	}
	
	sid varcontext::operator[](std::string str) const
	{
		return c.at(str);
	}
	
	void varcontext::print(std::ostream& s) const
	{
		for(const std::pair<const std::string, sid> p : c)
			s << p.second << ": " << p.first << std::endl;
	}
}

#include "symbols.h"

namespace splicpp
{
	boost::optional<uint> static_literal::match(const std::string source, const uint pos) const
	{
		if(source.length() < pos + str.length())
			return boost::optional<uint>();
		
		for(uint i = 0; i < str.length(); i++)
			if(source[pos+i] != str[i])
				return boost::optional<uint>();

		return str.size();
	}
	
	boost::optional<uint> regex_literal::match(const std::string source, const uint pos) const
	{
		boost::cmatch match;
		
		if(!boost::regex_match(source.substr(pos).c_str(), match, expr))
			return boost::optional<uint>();
		
		return std::string(match[1].first, match[1].second).size();
	}
	
	boost::optional<uint> end_literal::match(const std::string source, const uint pos) const
	{
		if(source.size() == pos)
			return boost::optional<uint>(0);

		return boost::optional<uint>();
	}
}

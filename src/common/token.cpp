#include "token.hpp"

namespace splicpp
{
	std::string token::as_string(const std::string source) const
	{
		return source.substr(pos, length);
	}
}

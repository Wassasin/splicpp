#ifndef ERRORS_H
#define ERRORS_H

#include <stdexcept>
#include <string>

namespace splicpp
{
	class parse_error : public std::runtime_error
	{
	public:
		parse_error(const std::string& what)
		: runtime_error(what)
		{}
	};
}

#endif

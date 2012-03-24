#ifndef ERRORS_H
#define ERRORS_H

#include <stdexcept>
#include <string>
#include <sstream>

#include "sloc.hpp"

namespace splicpp
{
	class parse_error : public std::runtime_error
	{
		static std::string create_message(const std::string& what, const sloc sl)
		{
			std::stringstream s;
			s << what;
			s << ", near line " << sl.line;
			return s.str();
		}
	
	public:
		parse_error(const std::string& what)
		: runtime_error(what)
		{}
	
		parse_error(const std::string& what, const sloc sl)
		: runtime_error(create_message(what, sl))
		{}
	};
}

#endif

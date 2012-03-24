#ifndef VARCONTEXT_H
#define VARCONTEXT_H

#include <map>
#include <string>
#include <ostream>

#include "../common/typedefs.hpp"

namespace splicpp
{
	class varcontext
	{
		std::map<std::string, sid> c;
		
	public:
		varcontext()
		: c()
		{}
		
		bool exists(const std::string str) const;
		void assign(const std::string str, const sid i);
		sid operator[](std::string str) const;
		void print(std::ostream& s) const;
	};
}

#endif

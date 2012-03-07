#ifndef IO_H
#define IO_H

#include <string>
#include <sstream>

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/convenience.hpp>

namespace splicpp
{
	static std::string readfile(boost::filesystem::path p)
	{
		if(!boost::filesystem::exists(p))
			throw std::runtime_error("File does not exist");
		
		boost::filesystem::fstream f(p);
		std::stringstream result;
		
		while(f)
		{
			std::string line;
			std::getline(f, line);
			result << line << '\n';
		}
		
		return result.str();
	}
}

#endif

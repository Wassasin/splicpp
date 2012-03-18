#ifndef UTILS_H
#define UTILS_H

#include <ostream>

#include <boost/optional.hpp>

namespace splicpp
{
	class delim_printer
	{
		const std::string delim;
		bool first;
		std::ostream& s;
		
	public:
		delim_printer(const std::string delim, std::ostream& s)
		: delim(delim)
		, first(true)
		, s(s)
		{}
		
		void print(const std::string str)
		{
			if(first)
				first = false;
			else
				s << delim;
			
			s << str;
		}
	};
	
	class delim_end_printer
	{
		const std::string delim, end;
		bool first, finished;
		std::ostream& s;
	
		boost::optional<std::string> buffer;
		
		void flush_buffer(const std::string marker)
		{
			if(first)
				first = false;
			else
				s << marker;
			
			s << buffer.get();
		}
		
	public:
		delim_end_printer(const std::string delim, const std::string end, std::ostream& s)
		: delim(delim)
		, end(end)
		, first(true)
		, finished(false)
		, s(s)
		, buffer()
		{}
		
		void print(const std::string str)
		{
			if(buffer)
				flush_buffer(delim);
			
			buffer = str;
		}
		
		void finish()
		{
			if(finished)
				return;
			
			finished = true;
			flush_buffer(end);
		}
		
		~delim_end_printer()
		{
			finish();
		}
	};
}

#endif

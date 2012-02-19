#ifndef LEXER_H
#define LEXER_H

#include <boost/optional.hpp>

#include "../common/token.h"
#include "../common/symbols.h"
#include "../common/typedefs.h"

namespace splicpp
{
	class lexer
	{
		grammar g;
		std::string str;

		uint i, line;
		
		void consume_garbage()
		{
			for(; i < str.size(); i++)
				if(!g.should_ignore(str[i]))
					return;
		}
		
	public:
		lexer(const grammar g, const std::string str)
		: g(g)
		, str(str)
		, i(0)
		, line(0)
		{}
		
		void reset()
		{
			i = 0;
			line = 0;
		}
		
		token next()
		{
			consume_garbage();
			boost::optional<token> tmp_t = g.try_match(str, i, line);
			
			if(!tmp_t)
			{
				std::cerr << "Can not match to tokens: \"" << str.substr(i, 10) << '"' << std::endl; //TODO
				throw std::exception();
			}
			
			token t = tmp_t.get();
			
			for(uint j = 0; j < t.length; j++)
				if(str[i+j] == '\n')
					line++;

			i += t.length;
			return t;
		}
		
		bool at_end() const
		{
			return(i >= str.size());
		}
		
		grammar get_grammar() const
		{
			return g;
		}
	};
}

#endif

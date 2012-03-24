#include "lexer.hpp"

#include <sstream>

namespace splicpp
{
	void lexer::consume_garbage()
	{
		for(; i < str.size(); i++)
		{
			if(str[i] == '\n')
				line++;
		
			if(!g.should_ignore(str[i]))
				return;
		}
	}

	void lexer::reset()
	{
		i = 0;
		line = 1;
	}
	
	token lexer::next()
	{
		consume_garbage();
		boost::optional<token> tmp_t;
		
		tmp_t = g.try_match(str, i, line);
		if(!tmp_t)
		{
			std::stringstream s;
			s << "Can not match to tokens: \"" << str.substr(i, 10) << '"' << std::endl;
			throw std::runtime_error(s.str());
		}
	
		token t = tmp_t.get();
		
		for(uint j = 0; j < t.length; j++)
			if(str[i+j] == '\n')
				line++;

		i += t.length;

		if(g.fetch_symbol(t.type)->type() == s_ignored)
			return next();

		return t;
	}
	
	bool lexer::at_end() const
	{
		return(i >= str.size());
	}
	
	grammar lexer::get_grammar() const
	{
		return g;
	}
	
	std::string lexer::get_str() const
	{
		return str;
	}
}

#include "lexer.hpp"

namespace splicpp
{
	void lexer::consume_garbage()
	{
		for(; i < str.size(); i++)
			if(!g.should_ignore(str[i]))
				return;
	}

	void lexer::reset()
	{
		i = 0;
		line = 0;
	}
	
	token lexer::next()
	{
		consume_garbage();
		boost::optional<token> tmp_t;
		
		tmp_t = g.try_match(str, i, line);
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

		if(g.fetch_symbol(t.type)->type() == s_ignored)
			return next();

		g.print_token(t, str);

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

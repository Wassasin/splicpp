#ifndef LEXER_H
#define LEXER_H

#include <boost/optional.hpp>

#include "../common/grammar.hpp"
#include "../common/token.hpp"

namespace splicpp
{
	class lexer
	{
		const grammar g;
		const std::string str;

		uint i, line;
		
		void consume_garbage();
		
	public:
		lexer(const grammar g, const std::string str)
		: g(g)
		, str(str)
		, i(0)
		, line(0)
		{}
		
		void reset();
		token next();
		
		bool at_end() const;
		grammar get_grammar() const;
		std::string get_str() const;
	};
}

#endif

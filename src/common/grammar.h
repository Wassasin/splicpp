#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <memory>

#include "symbols.h"
#include "rule.h"
#include "typedefs.h"

namespace splicpp
{
	class grammar
	{
		std::vector<std::shared_ptr<symbol>> symbols;
		std::vector<rule> rules;
		
		std::vector<char> ignore_list;

	public:
		grammar()
		: symbols()
		, rules()
		, ignore_list()
		{}
		
		virtual ~grammar() {}

		void ignore(char c)
		{
			ignore_list.push_back(c);
		}

		bool should_ignore(char c)
		{
			for(uint i = 0; i < ignore_list.size(); i++)
				if(ignore_list[i] == c)
					return true;

			return false;
		}

		stid add_symbol(symbol* s)
		{
			stid sid = symbols.size();
			symbols.push_back(std::shared_ptr<symbol>(s));
			return sid;
		}
		
		void add_rule(const rule r)
		{
			rules.push_back(r);
		}
		
		void print()
		{
			for(uint i = 0; i < rules.size(); i++)
				rules[i].print();
		}
		
		boost::optional<token> try_match(const std::string source, const uint pos, const uint line)
		{
			for(uint j = 0; j < symbols.size(); j++)
			{
				if(symbols[j]->type() != s_lit)
					continue;
				
				literal* lit = static_cast<literal*>(symbols[j].get()); //downcast, checked by enum type()
				boost::optional<uint> length = lit->match(source, pos);
				
				if(!length)
					continue;
				
				return token(j, pos, length.get(), line);
			}
			
			return boost::optional<token>();
		}
		
		void print_token(const token t, const std::string source)
		{
			std::cout << symbols[t.type]->name << " [" << source.substr(t.pos, t.length) << ']' << std::endl;
		}

		rule fetch_rule(rid i)
		{
			return rules[i];
		}

		std::shared_ptr<symbol> fetch_symbol(stid i)
		{
			return symbols[i];
		}
	};
}

#endif

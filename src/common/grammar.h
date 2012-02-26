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
		const stid S_EPSILON, NL_REAL_START, NL_START, L_END;
		const rid R_START;

		grammar()
		: symbols()
		, rules()
		, ignore_list()

		, S_EPSILON(add_symbol(new epsilon()))
		, NL_REAL_START(add_symbol(new non_literal("S'")))
		, NL_START(add_symbol(new non_literal("S")))
		, L_END(add_symbol(new end_literal()))

		, R_START(add_rule(rule(NL_REAL_START) + NL_START))
		{}
		
		virtual ~grammar() {}

		void ignore(const char c)
		{
			ignore_list.push_back(c);
		}

		bool should_ignore(const char c) const
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
		
		rid add_rule(rule r)
		{
			remove_epsilons(r);
			rid i = rules.size();
			rules.push_back(r);
			return i;
		}
		
		void remove_epsilons(rule& r)
		{
			std::vector<stid> new_body;
			for(size_t i = 0; i < r.body.size(); i++)
				if(r.body[i] != S_EPSILON)
					new_body.push_back(r.body[i]);
			
			r.body = new_body;
		}
		
		void print() const
		{
			for(uint i = 0; i < rules.size(); i++)
				rules[i].print();
		}
		
		boost::optional<token> try_match(const std::string source, const uint pos, const uint line) const
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
		
		void print_token(const token t, const std::string source) const
		{
			std::cout << symbols[t.type]->name << " [" << source.substr(t.pos, t.length) << ']' << std::endl;
		}

		rule fetch_rule(const rid i) const
		{
			return rules[i];
		}

		std::shared_ptr<symbol> fetch_symbol(const stid i) const
		{
			return symbols[i];
		}

		size_t rules_size() const
		{
			return rules.size();
		}

		size_t symbols_size() const
		{
			return symbols.size();
		}

		size_t nterminals_size() const
		{
			size_t result = 0;
			for(size_t i = 0; i < symbols.size(); i++)
				if(symbols[i]->type() == s_nlit)
					result++;

			return result;
		}

		size_t terminals_size() const
		{
			size_t result = 0;
			for(size_t i = 0; i < symbols.size(); i++)
				if(symbols[i]->type() == s_lit)
					result++;

			return result;
		}
	};
}

#endif

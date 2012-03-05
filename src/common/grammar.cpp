#include "grammar.hpp"

namespace splicpp
{
	void grammar::ignore(const char c)
	{
		ignore_list.push_back(c);
	}

	bool grammar::should_ignore(const char c) const
	{
		for(uint i = 0; i < ignore_list.size(); i++)
			if(ignore_list[i] == c)
				return true;

		return false;
	}

	stid grammar::add_symbol(symbol* s)
	{
		stid sid = symbols.size();
		symbols.push_back(std::shared_ptr<symbol>(s));
		return sid;
	}
	
	rid grammar::add_rule(rule r)
	{
		remove_epsilons(r);
		rid i = rules.size();
		rules.push_back(r);
		return i;
	}
	
	void grammar::remove_epsilons(rule& r)
	{
		std::vector<stid> new_body;
		for(size_t i = 0; i < r.body.size(); i++)
			if(r.body[i] != S_EPSILON)
				new_body.push_back(r.body[i]);
		
		r.body = new_body;
	}
	
	void grammar::print() const
	{
		for(rid i = 0; i < rules.size(); i++)
			print_rule(i);
	}
	
	boost::optional<token> grammar::try_match(const std::string source, const uint pos, const uint line) const
	{
		for(uint j = 0; j < symbols.size(); j++)
		{
			if(symbols.at(j)->type() != s_lit)
				continue;
			
			literal* lit = static_cast<literal*>(symbols.at(j).get()); //downcast, checked by enum type()
			boost::optional<uint> length = lit->match(source, pos);
			
			if(!length)
				continue;
			
			return token(j, pos, length.get(), line);
		}
		
		return boost::optional<token>();
	}
	
	void grammar::print_token(const token t, const std::string source) const
	{
		std::cout << symbols.at(t.type)->name << " [" << t.as_string(source) << ']' << std::endl;
	}
	
	void grammar::print_rule(const rid r) const
	{
		const rule rtmp = fetch_rule(r);
		
		std::cout << fetch_symbol(rtmp.start)->name << " :== ";
			
		for(uint i = 0; i < rtmp.body.size(); i++)
		{
			if(i > 0)
				std::cout << ' ';
				
			std::cout << fetch_symbol(rtmp.body.at(i))->name;
		}
		
		std::cout << std::endl;
	}

	const rule grammar::fetch_rule(const rid i) const
	{
		return rules.at(i);
	}
	
	stid grammar::fetch_stid(const std::string name) const
	{
		for(stid i = 0; i < symbols.size(); i++)
			if(symbols.at(i)->name == name)
				return i;
		
		throw std::runtime_error("Unknown symbol");
	}

	std::shared_ptr<symbol> grammar::fetch_symbol(const stid i) const
	{
		return symbols.at(i);
	}
	
	std::shared_ptr<symbol> grammar::fetch_symbol(const std::string name) const
	{
		return symbols.at(fetch_stid(name));
	}

	size_t grammar::rules_size() const
	{
		return rules.size();
	}

	size_t grammar::symbols_size() const
	{
		return symbols.size();
	}

	size_t grammar::nterminals_size() const
	{
		size_t result = 0;
		for(size_t i = 0; i < symbols.size(); i++)
			if(symbols.at(i)->type() == s_nlit)
				result++;

		return result;
	}

	size_t grammar::terminals_size() const
	{
		size_t result = 0;
		for(size_t i = 0; i < symbols.size(); i++)
			if(symbols.at(i)->type() == s_lit)
				result++;

		return result;
	}
	
	size_t grammar::translate_lit(const stid id) const
	{
		size_t result = 0;
		for(size_t i = 0; i < id; i++)
			if(symbols.at(i)->type() == s_lit)
				result++;
		
		return result;
	}
	
	size_t grammar::translate_nlit(const stid id) const
	{
		size_t result = 0;
		for(size_t i = 0; i < id; i++)
			if(symbols.at(i)->type() == s_nlit)
				result++;
		
		return result;
	}
}

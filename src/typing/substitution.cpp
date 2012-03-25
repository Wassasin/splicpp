#include "substitution.hpp"

#include <stdexcept>
#include <sstream>

#include "../common/utils.hpp"

#include "types/sl_type.hpp"
#include "types/sl_type_unbound.hpp"

namespace splicpp
{
	void substitution::add(const std::shared_ptr<sl_type_unbound> x, const std::shared_ptr<sl_type> y)
	{
		if(subs.size() > 0) //To break recursion of s.add below
		{
			for(const auto p : subs)
				if(p.first->equals(x))
					throw std::logic_error("Already contains substitution from x to y"); //TODO
		
			substitution s;
			s.add(x, y);
		
			for(auto& p : subs)
				p.second = p.second->apply(s); //Rewrite [z -> x] with [x -> y] to [z -> y]
		}
		
		subs.push_back(std::pair<std::shared_ptr<sl_type_unbound>, std::shared_ptr<sl_type>>(x, y)); //Unique, because of precondition as logic_error above
	}
	
	std::shared_ptr<sl_type> substitution::substitute(const std::shared_ptr<sl_type_unbound> x) const
	{
		for(const auto p : subs)
			if(p.first->equals(x))
				return p.second;
		
		return std::static_pointer_cast<sl_type>(x);
	}
	
	substitution substitution::composite(const substitution& s) const
	{
		substitution result;
		
		for(const auto p : s.subs)
			result.add(p.first, p.second);
		
		for(const auto p : subs)
			result.add(p.first, p.second);
		
		return result;
	}
	
	void substitution::print(std::ostream& s) const
	{
		s << '[';
		delim_printer p(", ", s);
		
		for(const auto sub : subs)
		{
			std::stringstream stmp;
			sub.first->print(stmp);
			stmp << " => ";
			sub.second->print(stmp);
			p.print(stmp.str());
		}
		
		s << ']';
	}
}

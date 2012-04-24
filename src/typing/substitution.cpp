#include "substitution.hpp"

#include <stdexcept>
#include <sstream>

#include "../common/utils.hpp"

#include "types/sl_type.hpp"
#include "types/sl_type_unbound.hpp"

#include "unification_error.hpp"

namespace splicpp
{
	void substitution::add(const s_ptr<sl_type_unbound> x, const s_ptr<sl_type> y)
	{
		if(y->type() == sl_type::t_unbound && x->equals(std::dynamic_pointer_cast<sl_type_unbound>(y))) // a == a
			return;
	
		if(subs.find(x) != subs.end()) // There already is a [x -> ?]
		{
			std::stringstream ss;
			ss << "Substitution already contains ";
			x->print(ss);
			ss << " => ";
			subs[x]->print(ss);
			ss << " (tried to add => ";
			y->print(ss);
			ss << ')' << std::endl;
			throw std::runtime_error(ss.str());
		}
		
		substitution s;
		s.subs[x] = y;
		
		decltype(subs) newsubs;
		
		for(const auto& p : subs)
		{
			newsubs[p.first] = p.second->apply(s); //Rewrite [z -> x] with [x -> y] to [z -> y]
			
			if(y->type() == sl_type::t_unbound && p.first->equals(x)) //Rewrite [z->a] [a->y] to [y->z]; aliasing
				newsubs[std::dynamic_pointer_cast<sl_type_unbound>(y)] = p.first->apply(s);
		}
		
		for(auto i = newsubs.begin(); i != newsubs.end(); ++i)
		{
			const auto& p = *i;
			if(p.second->type() == sl_type::t_unbound && p.first->equals(std::dynamic_pointer_cast<sl_type_unbound>(p.second)))
				newsubs.erase(i);
		}
		
		if(newsubs.find(x) == newsubs.end()) // If there is not yet a [x -> ?]
			newsubs[x] = y;
		
		subs = newsubs;
	}
	
	void substitution::set(const s_ptr<sl_type_unbound> x, const s_ptr<sl_type> y)
	{
		add(x, y);
	}
	
	s_ptr<sl_type> substitution::substitute(const s_ptr<sl_type_unbound> x) const
	{
		for(const auto p : subs)
			if(p.first->equals(x))
				return p.second;
		
		return x;
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

#include "typecontext.hpp"

#include <stdexcept>

#include "../common/generic.hpp"

#include "types/sl_polytype.hpp"
#include "types/sl_type_unbound.hpp"

namespace splicpp
{
	void typecontext::register_type(const sid i, const std::shared_ptr<sl_polytype> t)
	{
		if(i >= types.size())
			types.resize(i+1);
		
		types[i] = t;
	}
	
	bool typecontext::is_registered(const sid i) const
	{
		if(i >= types.size())
			return false;
		
		return types.at(i);
	}
	
	std::shared_ptr<sl_polytype> typecontext::operator[](const sid i) const
	{
		const auto t = types.at(i);
		if(!t)
			throw std::logic_error("Type not inserted into context");
		
		return t.get();
	}
	
	std::shared_ptr<sl_type_unbound> typecontext::create_fresh() const
	{
		return std::shared_ptr<sl_type_unbound>(new sl_type_unbound((*ft_count)++));
	}
	
	typecontext typecontext::apply(const substitution& s) const
	{
		typecontext c = *this;
		
		for(size_t i = 0; i < c.types.size(); i++)
			if(c.types[i])
				c.types[i] = c.types[i].get()->apply(c, s); //Original context
		
		return c;
	}
	
	typecontext typecontext::apply_maintain(const substitution& s) const
	{
		typecontext c = *this;
		
		for(size_t i = 0; i < c.types.size(); i++)
			if(c.types[i])
			{
				typecontext ctmp(ft_count);
				ctmp.register_type(i, types[i].get());
				c.types[i] = c.types[i].get()->apply(ctmp, s); //Maintain qualifications by creating an artificial empty typecontext
			}
			
		return c;
	}
	
	std::vector<std::shared_ptr<sl_type_unbound>> typecontext::fv() const
	{
		std::vector<std::shared_ptr<sl_type_unbound>> result;
		for(const auto t : types)
			if(t)
				for(const std::shared_ptr<sl_type_unbound> tv : t.get()->tv())
					if(!is_in_ptr<sl_type_unbound>(tv, result))
						result.push_back(tv);
		
		return result;
	}
	
	void typecontext::print(std::ostream& s) const
	{
		for(size_t i = 0; i < types.size(); i++)
		{
			s << i << ": ";
			if(types[i])
				types[i].get()->print(s);
			else
				s << "undef";
			
			s << std::endl;
		}
	}
	
	void typecontext::print_debug() const
	{
		print(std::cout);
	}
}

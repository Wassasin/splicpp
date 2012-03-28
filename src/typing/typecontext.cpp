#include "typecontext.hpp"

#include <stdexcept>

#include "types/sl_type.hpp"
#include "types/sl_type_unbound.hpp"

namespace splicpp
{
	void typecontext::register_type(const sid i, const std::shared_ptr<sl_type> t)
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
	
	std::shared_ptr<sl_type> typecontext::operator[](const sid i) const
	{
		const auto t = types.at(i);
		if(!t)
			throw std::logic_error("Type not inserted into context");
		
		return t.get();
	}
	
	std::shared_ptr<sl_type_unbound> typecontext::create_fresh()
	{
		return std::shared_ptr<sl_type_unbound>(new sl_type_unbound(ft_count++));
	}
}

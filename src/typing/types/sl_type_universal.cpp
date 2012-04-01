#include "sl_type_universal.hpp"

#include "sl_type.hpp"
#include "sl_type_unbound.hpp"

#include "../typecontext.hpp"

namespace splicpp
{	
	void sl_type_universal::bind(std::shared_ptr<sl_type_unbound> t)
	{
		bindings.push_back(t);
	}
	
	std::shared_ptr<sl_type> sl_type_universal::unbind(const typecontext& c) const
	{
		substitution s;
		for(const auto binding : bindings)
			s.add(binding, c.create_fresh());
		
		return t->apply(s);
	}

	sl_type::sl_type_type sl_type_universal::type() const
	{
		return t_universal;
	}
	
	void sl_type_universal::print(std::ostream& s) const
	{
		s << "forall";
		for(const auto b : bindings)
			b->print(s << ' ');
		
		t->print(s << " . ");
	}
	
	std::vector<std::shared_ptr<sl_type_unbound>> sl_type_universal::tv() const
	{
		auto result = t->tv();
		
		for(const auto b : bindings)
			for(size_t i = 0; i < result.size(); i++)
				if(b->equals(result[i]))
					result.erase(result.begin()+i);
		
		return result;
	}
	
	substitution sl_type_universal::unify(const std::shared_ptr<sl_type> t) const
	{
		return this->t->unify(t);
	}
	
	std::shared_ptr<sl_type> sl_type_universal::apply(const substitution& s) const
	{
		const auto tt = t->apply(s);
		const std::shared_ptr<sl_type_universal> result(new sl_type_universal(tt));
		
		for(const auto b : bindings)
		{
			const auto bt = b->apply(s);
			if(bt->is_unbound())
				result->bind(std::dynamic_pointer_cast<sl_type_unbound>(bt));
		}
		
		if(result->bindings.size() == 0)
			return tt;
		
		return std::static_pointer_cast<sl_type>(result);
	}
}
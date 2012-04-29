#include "ast_construct.hpp"

#include <vector>

#include "../common/generic.hpp"

#include "../typing/typecontext.hpp"
#include "../typing/types/sl_polytype.hpp"
#include "../typing/types/sl_type_array.hpp"
#include "../typing/types/sl_type_bool.hpp"
#include "../typing/types/sl_type_function.hpp"
#include "../typing/types/sl_type_tuple.hpp"
#include "../typing/types/sl_type_unbound.hpp"
#include "../typing/types/sl_type_void.hpp"

namespace splicpp
{
	void ast_construct::pretty_print(std::ostream& s, const uint) const
	{
		s << "__INTERNAL__";
	}
	
	void ast_construct::assign(const sid i)
	{
		id = i;
	}
	
	substitution ast_construct::declare_type(typecontext& c) const
	{
		c.register_type(id.get(), sl_polytype::qualify(c, fetch_type(c)));
		return substitution::id();
	}
	
	/* print, isEmpty, head, tail, fst, and snd */
	
	std::string ast_construct_print::fetch_name() const
	{
		return "print";
	}
	
	s_ptr<const sl_type> ast_construct_print::fetch_type(const typecontext& c) const
	{
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			std::static_pointer_cast<const sl_type>(c.create_fresh(sl))
		));
		
		const s_ptr<const sl_type> r(new sl_type_void(sl));
		
		return s_ptr<const sl_type>(new sl_type_function(args, r, sl));
	}
	
	std::string ast_construct_is_empty::fetch_name() const
	{
		return "isEmpty";
	}
	
	s_ptr<const sl_type> ast_construct_is_empty::fetch_type(const typecontext& c) const
	{
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			s_ptr<const sl_type>(new sl_type_array(std::static_pointer_cast<const sl_type>(c.create_fresh(sl)), sl))
		));
		
		const s_ptr<const sl_type> r(new sl_type_bool(sl));
		
		return s_ptr<const sl_type>(new sl_type_function(args, r, sl));
	}
	
	std::string ast_construct_head::fetch_name() const
	{
		return "head";
	}
	
	s_ptr<const sl_type> ast_construct_head::fetch_type(const typecontext& c) const
	{
		const s_ptr<const sl_type> a = std::static_pointer_cast<const sl_type>(c.create_fresh(sl));
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			s_ptr<const sl_type>(new sl_type_array(a, sl))
		));
		
		return s_ptr<const sl_type>(new sl_type_function(args, a, sl));
	}
	
	std::string ast_construct_tail::fetch_name() const
	{
		return "tail";
	}
	
	s_ptr<const sl_type> ast_construct_tail::fetch_type(const typecontext& c) const
	{
		const s_ptr<const sl_type> r(new sl_type_array(c.create_fresh(sl), sl));
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			s_ptr<const sl_type>(r)
		));
		
		return s_ptr<const sl_type>(new sl_type_function(args, r, sl));
	}
	
	std::string ast_construct_fst::fetch_name() const
	{
		return "fst";
	}
	
	s_ptr<const sl_type> ast_construct_fst::fetch_type(const typecontext& c) const
	{
		const s_ptr<const sl_type> a1 = std::static_pointer_cast<const sl_type>(c.create_fresh(sl));
		const s_ptr<const sl_type> a2 = std::static_pointer_cast<const sl_type>(c.create_fresh(sl));
		
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			s_ptr<const sl_type>(new sl_type_tuple(a1, a2, sl))
		));
		
		return s_ptr<const sl_type>(new sl_type_function(args, a1, sl));
	}
	
	std::string ast_construct_snd::fetch_name() const
	{
		return "snd";
	}
	
	s_ptr<const sl_type> ast_construct_snd::fetch_type(const typecontext& c) const
	{
		const s_ptr<const sl_type> a1 = std::static_pointer_cast<const sl_type>(c.create_fresh(sl));
		const s_ptr<const sl_type> a2 = std::static_pointer_cast<const sl_type>(c.create_fresh(sl));
		
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			s_ptr<const sl_type>(new sl_type_tuple(a1, a2, sl))
		));
		
		return s_ptr<const sl_type>(new sl_type_function(args, a2, sl));
	}
}

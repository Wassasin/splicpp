#include "ast_construct.hpp"

#include <vector>

#include "../typing/typecontext.hpp"
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
	
	/* print, isEmpty, head, tail, fst, and snd */
	
	std::string ast_construct_print::fetch_name() const
	{
		return "print";
	}
	
	std::shared_ptr<sl_type> ast_construct_print::fetch_type(typecontext& c) const
	{
		const std::vector<std::shared_ptr<sl_type>> args({ std::static_pointer_cast<sl_type>(c.create_fresh()) });
		const std::shared_ptr<sl_type> r(new sl_type_void());
		
		return std::shared_ptr<sl_type>(new sl_type_function(args, r))->qualify(c);
	}
	
	std::string ast_construct_is_empty::fetch_name() const
	{
		return "isEmpty";
	}
	
	std::shared_ptr<sl_type> ast_construct_is_empty::fetch_type(typecontext& c) const
	{
		const std::vector<std::shared_ptr<sl_type>> args({ std::shared_ptr<sl_type>(new sl_type_array(std::static_pointer_cast<sl_type>(c.create_fresh()))) });
		const std::shared_ptr<sl_type> r(new sl_type_bool());
		
		return std::shared_ptr<sl_type>(new sl_type_function(args, r))->qualify(c);
	}
	
	std::string ast_construct_head::fetch_name() const
	{
		return "head";
	}
	
	std::shared_ptr<sl_type> ast_construct_head::fetch_type(typecontext& c) const
	{
		const std::shared_ptr<sl_type> a = std::static_pointer_cast<sl_type>(c.create_fresh());
		const std::vector<std::shared_ptr<sl_type>> args({ std::shared_ptr<sl_type>(new sl_type_array(a)) });
		
		return std::shared_ptr<sl_type>(new sl_type_function(args, a))->qualify(c);
	}
	
	std::string ast_construct_tail::fetch_name() const
	{
		return "tail";
	}
	
	std::shared_ptr<sl_type> ast_construct_tail::fetch_type(typecontext& c) const
	{
		const std::shared_ptr<sl_type> r(new sl_type_array(c.create_fresh()));
		const std::vector<std::shared_ptr<sl_type>> args({ std::shared_ptr<sl_type>(r) });
		
		return std::shared_ptr<sl_type>(new sl_type_function(args, r))->qualify(c);
	}
	
	std::string ast_construct_fst::fetch_name() const
	{
		return "fst";
	}
	
	std::shared_ptr<sl_type> ast_construct_fst::fetch_type(typecontext& c) const
	{
		const std::shared_ptr<sl_type> a1 = std::static_pointer_cast<sl_type>(c.create_fresh());
		const std::shared_ptr<sl_type> a2 = std::static_pointer_cast<sl_type>(c.create_fresh());
		
		const std::vector<std::shared_ptr<sl_type>> args({ std::shared_ptr<sl_type>(new sl_type_tuple(a1, a2)) });
		
		return std::shared_ptr<sl_type>(new sl_type_function(args, a1))->qualify(c);
	}
	
	std::string ast_construct_snd::fetch_name() const
	{
		return "snd";
	}
	
	std::shared_ptr<sl_type> ast_construct_snd::fetch_type(typecontext& c) const
	{
		const std::shared_ptr<sl_type> a1 = std::static_pointer_cast<sl_type>(c.create_fresh());
		const std::shared_ptr<sl_type> a2 = std::static_pointer_cast<sl_type>(c.create_fresh());
		
		const std::vector<std::shared_ptr<sl_type>> args({ std::shared_ptr<sl_type>(new sl_type_tuple(a1, a2)) });
		
		return std::shared_ptr<sl_type>(new sl_type_function(args, a2))->qualify(c);
	}
}

#include "ast_construct.hpp"

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
	
	std::string ast_construct_is_empty::fetch_name() const
	{
		return "isEmpty";
	}
	
	std::string ast_construct_head::fetch_name() const
	{
		return "head";
	}
	
	std::string ast_construct_tail::fetch_name() const
	{
		return "tail";
	}
	
	std::string ast_construct_fst::fetch_name() const
	{
		return "fst";
	}
	
	std::string ast_construct_snd::fetch_name() const
	{
		return "snd";
	}
}

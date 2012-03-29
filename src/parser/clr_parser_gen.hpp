#ifndef CLR_PARSER_GEN_H
#define CLR_PARSER_GEN_H

#include <boost/function.hpp>

#include "ptable.hpp"
#include "itemset.hpp"

namespace splicpp
{
	class clr_parser_gen
	{
		clr_parser_gen() {}

	public:
		typedef boost::function<void (std::vector<ptable::acttransition>& transitions, const size_t i, const std::vector<itemset<1>> c, const stid a, const grammar& g)> conflict_resolver;
		static void resolve_nothing(std::vector<ptable::acttransition>&, const size_t, const std::vector<itemset<1>>, const stid, const grammar) {}
	
		static ptable generate(const grammar& g);
		static ptable generate(const grammar& g, const conflict_resolver f); //dragon book, page 265
		static ptable::acttransition generate_act(const size_t i_set_i, const std::vector<itemset<1>>& c, const boost::optional<stateid> forwards_to, const stid a, const grammar& g, const conflict_resolver f);
		static ptable::gototransition generate_goto(const boost::optional<stateid> forwards_to); //dragon book, page 265
		static itemset<1> closure(itemset<1> i_set, const std::vector<std::vector<stid>>& first_cache, const grammar& g); //dragon book, page 261
		static boost::optional<std::vector<stid>> closure_first(const item<1> i, const std::vector<std::vector<stid>>& first_cache, const grammar& g);
		static itemset<1> goto_f(const itemset<1>& i_set, const stid x, const std::vector<std::vector<stid>>& first_cache, const grammar& g);
		static std::pair<std::vector<itemset<1>>, std::vector<std::vector<boost::optional<stateid>>>> items(const grammar& g); //dragon book, page 261
		static std::vector<std::vector<stid>> create_first_cache(const grammar& g);
	};
}

#endif

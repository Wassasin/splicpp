#ifndef CLR_PARSER_GEN_H
#define CLR_PARSER_GEN_H

#include <boost/function.hpp>

#include "slr_parser_gen.hpp"

namespace splicpp
{
	class clr_parser_gen
	{
		clr_parser_gen() {}

	public:
		typedef boost::function<void (std::vector<ptable::acttransition>& transitions, const size_t i, const std::vector<itemset<1>> c, const stid a, const grammar& g)> conflict_resolver;
		static void resolve_nothing(std::vector<ptable::acttransition>&, const size_t, const std::vector<itemset<1>>, const stid, const grammar) {}
	
		static ptable generate(const grammar& g)
		{
			return generate(g, resolve_nothing);
		}
	
		static ptable generate(const grammar& g, const conflict_resolver f) //dragon book, page 265
		{
			const size_t terminals = g.terminals_size(), nterminals = g.nterminals_size();
		
			const auto pair = items(g);
			const auto& c = pair.first;
			const auto& forwards_to = pair.second;
			
			ptable result(terminals, nterminals);
			
			for(size_t i = 0; i < c.size(); i++)
			{
				std::vector<ptable::acttransition> actrow;
				std::vector<ptable::gototransition> gotorow;
				
				for(stid a = 0; a < g.symbols_size(); a++)
				{
					if(g.fetch_symbol(a)->type() == s_lit)
						actrow.push_back(generate_act(i, c, forwards_to[i][a], a, g, f));
					else if(g.fetch_symbol(a)->type() == s_nlit)
						gotorow.push_back(generate_goto(forwards_to[i][a]));
				}
				
				result.add_state(actrow, gotorow);
			}
			
			return result;
		}
		
		static ptable::acttransition generate_act(const size_t i_set_i, const std::vector<itemset<1>>& c, const boost::optional<stateid> forwards_to, const stid a, const grammar& g, const conflict_resolver f)
		{
			const itemset<1>& i_set = c[i_set_i];
			std::vector<ptable::acttransition> result;
			
			//case 2(a)
			if(forwards_to)
				for(const item<1>& i : i_set)
					if(!i.at_end(g) && i.after_dot(g) == a)
						result.push_back(ptable::acttransition::shift(forwards_to.get()));
			
			//case 2(b)
			for(const item<1>& i : i_set)
			{
				stid start = g.fetch_rule(i.rule).start;
				if(start != g.NL_REAL_START && i.at_end(g) && i.lookahead[0] == a)
					result.push_back(ptable::acttransition::reduce(i.rule));
			}
			
			//case 2(c)
			if(a == g.L_END)
				for(const item<1>& i : i_set)
					if(i.at_end(g) && i.rule == g.R_START && i.lookahead[0] == g.L_END)
						result.push_back(ptable::acttransition::accept());
			
			//remove duplicates
			remove_duplicates<ptable::acttransition>(result);
			
			if(result.size() == 0)
				return ptable::acttransition::error();
			
			if(result.size() > 1)
				f(result, i_set_i, c, a, g);
			
			if(result.size() > 1)
				throw std::runtime_error("Parser is not CLR(1)");
			
			return result[0];
		}
		
		static ptable::gototransition generate_goto(const boost::optional<stateid> forwards_to) //dragon book, page 265
		{
			if(forwards_to)
				return ptable::gototransition::jump(forwards_to.get());
			
			return ptable::gototransition::error();
		}
		
		static itemset<1> closure(itemset<1> i_set, const grammar& g) //dragon book, page 261
		{
			const size_t rsize = g.rules_size();
			for(size_t i = 0; i < i_set.size(); i++) //i_set changes size, thus foreach does not cut it
			{
				const auto& first = closure_first(i_set[i], g);
				if(!first)
					continue;
				
				const stid b = i_set[i].after_dot(g);
				
				for(rid j = 0; j < rsize; j++)
				{
					const rule& r = g.fetch_rule(j);
					if(r.start != b)
						continue;
					
					for(const stid bfirst : first.get())
					{
						if(g.fetch_symbol(bfirst)->type() != s_lit)
							continue;
					
						const auto i_new = item<1>(j, 0, { { bfirst } });
						
						if(!i_set.contains(i_new))
							i_set.push_back(i_new);
					}
				}
			}

			return i_set;
		}
		
		static boost::optional<std::vector<stid>> closure_first(const item<1> i, const grammar& g)
		{
			if(i.at_end(g))
				return boost::optional<std::vector<stid>>();
		
			const stid b = i.after_dot(g);
			
			if(g.fetch_symbol(b)->type() != s_nlit)
				return boost::optional<std::vector<stid>>();
		
			std::vector<stid> sentence = i.next(g).remainder(g);
			sentence.insert(sentence.end(), i.lookahead.begin(), i.lookahead.end());

			return slr_parser_gen::first(sentence, g);
		}
		
		template <size_t L>
		static itemset<L> goto_f(const itemset<L>& i_set, const stid x, const grammar& g)
		{
			itemset<L> preselection;
			for(size_t i = 0; i < i_set.size(); i++)
			{
				const item<L> i_item = i_set[i];

				if(i_item.at_end(g))
					continue;

				if(i_item.after_dot(g) != x)
					continue;

				preselection.push_back(i_item.next(g));
			}

			return closure(preselection, g);
		}
		
		static std::pair<std::vector<itemset<1>>, std::vector<std::vector<boost::optional<stateid>>>> items(const grammar& g) //dragon book, page 261
		{
			std::vector<itemset<1>> c = {closure({ item<1>(g.R_START, 0, { { g.L_END } }) }, g)};
			std::vector<std::vector<boost::optional<stateid>>> f;
			
			//the repeat as described in the dragon book is unnecessary, already captured by the random access and c.size
			for(stateid i = 0; i < c.size(); i++)
			{
				std::vector<boost::optional<stateid>> forwarding;
				forwarding.reserve(g.symbols_size());
				
				for(stid x = 0; x < g.symbols_size(); x++)
				{
					auto goto_set = goto_f<1>(c[i], x, g);
					
					if(goto_set.size() == 0)
					{
						forwarding.push_back(boost::optional<stateid>());
						continue;
					}
					
					bool found = false;
					for(size_t j = 0; j < c.size(); j++)
						if(goto_set == c[j])
						{
							assert(!found);
							forwarding.push_back(j);
							found = true;
						}
					
					if(found)
						continue;
					
					forwarding.push_back(c.size());
					c.push_back(goto_set);
				}
				
				f.push_back(forwarding);
			}
			
			return std::pair<std::vector<itemset<1>>, std::vector<std::vector<boost::optional<stateid>>>>(c, f);
		}
	};
}

#endif

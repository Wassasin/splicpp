#ifndef CLR_PARSER_GEN_H
#define CLR_PARSER_GEN_H

#include "slr_parser_gen.hpp"

namespace splicpp
{
	class clr_parser_gen
	{
		clr_parser_gen() {}

	public:
		static ptable generate(const grammar g) //dragon book, page 265
		{
			const size_t terminals = g.terminals_size(), nterminals = g.nterminals_size();
		
			auto c = items(g);
			ptable result(terminals, nterminals);
			
			for(size_t i = 0; i < c.size(); i++)
			{
				std::vector<ptable::acttransition> actrow;
				std::vector<ptable::gototransition> gotorow;
				
				for(stid a = 0; a < g.symbols_size(); a++)
					if(g.fetch_symbol(a)->type() == s_lit)
						actrow.push_back(generate_act(c.at(i), c, a, g));
					else if(g.fetch_symbol(a)->type() == s_nlit)
						gotorow.push_back(generate_goto(c.at(i), c, a, g));
				
				result.add_state(actrow, gotorow);
			}
			
			return result;
		}
		
		static ptable::acttransition generate_act(const itemset<1> i_set, const std::vector<itemset<1>> c, const stid a, const grammar g)
		{
			std::vector<ptable::acttransition> result;
		
			//case 2(a)
			for(size_t i = 0; i < i_set.size(); i++)
				if(!i_set[i].at_end(g) && i_set[i].after_dot(g) == a)
				{
					auto goto_set = goto_f<1>(i_set, a, g);
					
					for(stateid j = 0; j < c.size(); j++)
						if(goto_set == c.at(j))
							result.push_back(ptable::acttransition::shift(j));
				}
			
			//case 2(b)
			for(size_t i = 0; i < i_set.size(); i++)
			{
				stid start = g.fetch_rule(i_set[i].rule).start;
				if(start != g.NL_REAL_START && i_set[i].at_end(g) && i_set[i].lookahead[0] == a)
					result.push_back(ptable::acttransition::reduce(i_set[i].rule));
			}
			
			//case 2(c)
			if(a == g.L_END)
				for(size_t i = 0; i < i_set.size(); i++)
					if(i_set[i].at_end(g) && i_set[i].rule == g.R_START && i_set[i].lookahead[0] == g.L_END)
						result.push_back(ptable::acttransition::accept());
			
			//remove duplicates
			remove_duplicates<ptable::acttransition>(result);
			
			if(result.size() == 0)
				return ptable::acttransition::error();
			
			if(result.size() > 1)
				throw std::runtime_error("Parser is not CLR(1)");
			
			return result[0];
		}
		
		static ptable::gototransition generate_goto(const itemset<1> i_set, const std::vector<itemset<1>> c, const stid a, const grammar g) //dragon book, page 265
		{
			auto goto_set = goto_f<1>(i_set, a, g);
			for(stateid j = 0; j < c.size(); j++)
				if(goto_set == c.at(j))
					return ptable::gototransition::jump(j);
			
			return ptable::gototransition::error();
		}
		
		static itemset<1> closure(itemset<1> i_set, const grammar g) //dragon book, page 261
		{
			bool changed;
			do
			{
				changed = false;

				for(const item<1> i : i_set)
				{
					if(i.at_end(g))
						continue;

					const stid b = i.after_dot(g);
					
					if(g.fetch_symbol(b)->type() != s_nlit)
						continue;

					std::vector<stid> sentence = i.next(g).remainder(g);
					for(const stid a : i.lookahead)
						sentence.push_back(a);

					const std::vector<stid> first_set = slr_parser_gen::first(sentence, g);

					for(size_t j = 0; j < g.rules_size(); j++)
					{
						rule r = g.fetch_rule(j);
						if(r.start != b)
							continue;
						
						for(const stid bfirst : first_set)
						{
							if(g.fetch_symbol(bfirst)->type() != s_lit)
								continue;
						
							const auto i_new = item<1>(j, 0, { { bfirst } });
							
							if(!i_set.contains(i_new))
							{
								i_set.push_back(i_new);
								changed = true;
							}
						}
					}
					
					if(changed)
						break;
				}
			} while(changed);

			return i_set;
		}
		
		template <size_t L>
		static itemset<L> goto_f(const itemset<L> i_set, const stid x, const grammar g)
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
		
		static std::vector<itemset<1>> items(const grammar g) //dragon book, page 261
		{
			std::vector<itemset<1>> c = {closure({ item<1>(g.R_START, 0, { { g.L_END } }) }, g)};
			
			bool changed;
			do
			{
				changed = false;

				for(const itemset<1> i_set : c)
				{
					for(stid x = 0; x < g.symbols_size(); x++)
					{
						auto goto_set = goto_f<1>(i_set, x, g);
						if(goto_set.size() == 0 || goto_set.is_in(c))
							continue;
						
						c.push_back(goto_set);
						changed = true;
					}
					
					if(changed)
						break;
				}
			} while(changed);
			
			return c;
		}
	};
}

#endif

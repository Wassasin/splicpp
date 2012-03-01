#ifndef SLR_PARSER_GEN_H
#define SLR_PARSER_GEN_H

#include "../common/typedefs.h"
#include "../common/generic.h"

#include "ptable.h"
#include "item.h"
#include "itemset.h"

namespace splicpp
{
	class slr_parser_gen
	{
		slr_parser_gen() {}

	public:
		static ptable generate(const grammar g) //dragon book, page 253
		{
			const size_t terminals = g.terminals_size(), nterminals = g.nterminals_size();
		
			auto c = items(g);
			ptable result(terminals, nterminals);
			
			for(size_t i = 0; i < c.size(); i++)
			{
				std::vector<ptable::acttransition> actrow(terminals);
				std::vector<ptable::gototransition> gotorow(nterminals);

				for(stid a = 0; a < g.symbols_size(); a++)
					if(g.fetch_symbol(a)->type() == s_lit)
						actrow.push_back(generate_act(c.at(i), c, a, g));
			//		else if(g.fetch_symbol(a)->type() == s_nlit)
			//			gotorow.push_back(generate_goto(c[i], c, a, g));
					
				result.add_state(actrow, gotorow);
			}
			
			return result;
		}
		
		static ptable::acttransition generate_act(const itemset<0> i_set, const std::vector<itemset<0>> c, const stid a, const grammar g)
		{
			std::vector<ptable::acttransition> result;
		
			//case 2(a)
			for(size_t i = 0; i < i_set.size(); i++)
				if(!i_set[i].at_end(g) && i_set[i].after_dot(g) == a)
				{
					auto goto_set = goto_f<0>(i_set, a, g);
					
					for(stateid j = 0; j < c.size(); j++)
						if(goto_set == c.at(j))
							result.push_back(ptable::acttransition::shift(j));
				}
			
			//case 2(b)
			for(size_t i = 0; i < i_set.size(); i++)
			{
				stid start = g.fetch_rule(i_set[i].rule).start;
				if(start != g.NL_REAL_START && i_set[i].at_end(g) && is_in<stid>(a, follow(start, g)))
					result.push_back(ptable::acttransition::reduce(i_set[i].rule));
			}
			
			//case 2(c)
			if(a == g.L_END)
				for(size_t i = 0; i < i_set.size(); i++)
					if(i_set[i].at_end(g) && i_set[i].rule == g.R_START)
						result.push_back(ptable::acttransition::accept());
			
			if(result.size() == 0)
				return ptable::acttransition::error();
			
			if(result.size() > 1)
				throw std::exception(); //TODO - not SLR(1)
			
			return result[0];
		}
		
		static ptable::gototransition generate_goto(const itemset<0> i_set, const std::vector<itemset<0>> c, const stid a, const grammar g)
		{
			auto goto_set = goto_f<0>(i_set, a, g);
			for(stateid j = 0; j < c.size(); j++)
				if(goto_set == c.at(j))
					return ptable::gototransition::jump(j);
			
			throw std::exception();
		}
		
		static std::vector<stid> first(const stid a, const grammar g) //dragon book, page 221
		{
			std::vector<stid> result;
			
			auto s = g.fetch_symbol(a);
			if(s->type() == s_lit) //case 1
				result.push_back(a);
			else if(s->type() == s_nlit)
			{
				for(rid i = 0; i < g.rules_size(); i++)
				{
					const rule r = g.fetch_rule(i);
					if(r.start != a)
						continue;
					
					if(r.body.size() > 0) //case 2
					{
						for(size_t j = 0; j < r.body.size(); j++)
						{
							bool has_epsilon = false;
							auto first_j = first(r.body[j], g);
							for(size_t k = 0; k < first_j.size(); k++)
							{
								const stid b = first_j[k];
								if(b != g.S_EPSILON)
									result.push_back(b);
								else
									has_epsilon = true;
							}
							
							if(!has_epsilon)
								break;
							else if(j == r.body.size()-1)
								result.push_back(g.S_EPSILON);
						}
					}
					else //case 3
						result.push_back(g.S_EPSILON);
				}
				
				remove_duplicates<stid>(result);
				return result;
			}
			
			return result;
		}
		
		static std::vector<stid> first(const std::vector<stid> as, const grammar g)
		{
			std::vector<stid> result;
			
			size_t until = 1;
			for(size_t i = 0; i < as.size() && i < until; i++)
			{
				const auto first_set = first(as[i], g);
				for(size_t j = 0; j < first_set.size(); j++)
				{
					const stid x = first_set[j];
					if(x == g.S_EPSILON)
					{
						if(until == i+1)
							until++;
						
						if(i == as.size()-1)
							result.push_back(g.S_EPSILON);
					}
					else
						result.push_back(x);
				}
			}
			
			remove_duplicates<stid>(result);
			return result;
		}
		
		static std::vector<stid> follow(const stid a, const grammar g) //dragon book, page 221
		{
			assert(g.fetch_symbol(a)->type() == s_nlit);
			
			std::vector<stid> result;
			
			if(a == g.NL_START) //case 1
				result.push_back(g.L_END);
			
			for(rid i = 0; i < g.symbols_size(); i++)
			{
				const rule r = g.fetch_rule(i);
				for(size_t j = 0; j < r.body.size(); j++)
					if(r.body[j] == a)
					{
						bool last_case = false;
						if(j < r.body.size()-1) //case 2
						{
							auto first_set = first(subvector<stid>(r.body, j+1), g);
							for(size_t k = 0; k < first_set.size(); k++)
								if(first_set[k] != g.S_EPSILON)
									result.push_back(first_set[k]);
								else
									last_case = true;
						}
						else
							last_case = true;
						
						if(last_case) //case 3
						{
							auto follow_set = follow(r.start, g);
							for(size_t k = 0; k < follow_set.size(); k++)
								result.push_back(follow_set[k]);
						}
					}
			}
			
			remove_duplicates<stid>(result);
			return result;
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

			return closure<L>(preselection, g);
		}
		
		static std::vector<itemset<0>> items(const grammar g) //dragon book, page 246
		{
			std::vector<itemset<0>> c;

			itemset<0> init_set;
			init_set.push_back(item<0>(g.R_START, 0));

			c.push_back(closure<0>(init_set, g));
			
			//TODO REMOVE
			std::cout << "-----" << std::endl << "after closure" << std::endl;
			itemset<0>::print(c, g);
			std::cout << std::endl;
			//END TODO REMOVE
			
			bool changed;
			do
			{
				changed = false;

				for(size_t i = 0; i < c.size(); i++)
				{
					auto i_set = c.at(i);
					
					for(stid x = 0; x < g.symbols_size(); x++)
					{
						auto goto_set = goto_f<0>(i_set, x, g);
						if(goto_set.size() == 0 || goto_set.is_in(c, g))
							continue;
						
						c.push_back(goto_set);
						
						//TODO REMOVE
						for(size_t j = 0; j < goto_set.size(); j++)
						{
							goto_set[j].print(g);
							std::cout << "\t";
						}
							
						std::cout << std::endl;
						//END TODO REMOVE
						
						changed = true;
					}
				}
			} while(changed);

			return c;
		}

		template <size_t L>
		static itemset<L> closure(const itemset<L> i_set, const grammar g) //dragon book, page 245
		{
			itemset<L> j_set(i_set);

			std::vector<bool> added(g.symbols_size());
			for(size_t i = 0; i < g.symbols_size(); i++)
				added[i] = false;
			
			bool changed;
			do
			{
				changed = false;

				for(size_t i = 0; i < j_set.size(); i++)
				{
					if(j_set[i].at_end(g))
						continue;

					stid b = j_set[i].after_dot(g);
					
					if(g.fetch_symbol(b)->type() != s_nlit)
						continue;

					if(added[b])
						continue;

					for(size_t j = 0; j < g.rules_size(); j++)
					{
						rule r = g.fetch_rule(j);
						if(r.start != b)
							continue;
						
						j_set.push_back(item<L>(j, 0));
						changed = true;
					}
					
					added[b] = true;
				}
			} while(changed);

			return j_set;
		}
	};
}

#endif

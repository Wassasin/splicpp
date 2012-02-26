#ifndef SLR_PARSER_GEN_H
#define SLR_PARSER_GEN_H

#include "../common/typedefs.h"
#include "../common/generic.h"

#include "ptable.h"
#include "item.h"

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
				std::vector<ptable::transition> actrow(terminals);
				std::vector<stid> gotorow(nterminals);

				for(stid a = 0; a < actrow.size(); a++)
					actrow.push_back(generate_act(c[i], c, a, g));
					
				result.add_state(actrow, gotorow);
			}
			
			return result;
		}
		
		static ptable::transition generate_act(const std::vector<item<0>> i_set, const std::vector<std::vector<item<0>>> c, const stid a, const grammar g)
		{
			std::vector<ptable::transition> result;
		
			//case 2(a)
			for(size_t i = 0; i < i_set.size(); i++)
				if(!i_set[i].at_end(g) && i_set[i].after_dot(g) == a)
				{
					auto goto_set = goto_f<0>(i_set, a, g);
					
					for(stateid j = 0; j < c.size(); j++)
						if(item_equals(goto_set, c[j]))
							result.push_back(ptable::transition::shift(j));
				}
			
			//case 2(b)
			for(size_t i = 0; i < i_set.size(); i++)
			{
				stid start = g.fetch_rule(i_set[i].rule).start;
				if(start != g.NL_REAL_START && i_set[i].at_end(g) && is_in<stid>(a, follow(start)))
					result.push_back(ptable::transition::reduce(i_set[i].rule));
			}
			
			//case 2(c)
			if(a == g.L_END)
				for(size_t i = 0; i < i_set.size(); i++)
					if(i_set[i].at_end(g) && i_set[i].rule == g.R_START)
						result.push_back(ptable::transition::accept());
			
			if(result.size() == 0)
				return ptable::transition::error();
			
			if(result.size() > 1)
				throw std::exception(); //TODO - not SLR(1)
			
			return result[0];
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
				const auto first_set = first(as[i]);
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
		
		static std::vector<stid> follow(const stid a) //dragon book, page 221
		{
			//
		}
	
		template <size_t L>
		static std::vector<item<L>> goto_f(const std::vector<item<L>> i_set, const stid x, const grammar g)
		{
			std::vector<item<L>> preselection;
			for(size_t i = 0; i < i_set.size(); i++)
			{
				const item<L> i_item = i_set[i];

				if(i_item.at_end(g))
					continue;

				if(i_item.after_dot(g) != x)
					continue;

				preselection.push_back(i_item);
			}

			return closure<L>(preselection, g);
		}

		template <size_t L>
		static bool item_is_in(const std::vector<item<L>> i_set, const std::vector<std::vector<item<L>>> c)
		{
			for(size_t i = 0; i < c.size(); i++)
				if(item_equals(c[i], i_set))
					return true;

			return false;
		}

		template <size_t L>
		static bool item_equals(const std::vector<item<L>> i_set, const std::vector<item<L>> j_set)
		{
			if(i_set.size() != j_set.size())
				return false;
			
			for(size_t i = 0; i < i_set.size(); i++)
				if(!is_in<item<L>>(i_set[i], j_set))
					return false;

			return true;
		}

		static std::vector<std::vector<item<0>>> items(const grammar g) //dragon book, page 246
		{
			std::vector<std::vector<item<0>>> c;

			std::vector<item<0>> init_set;
			init_set.push_back(item<0>(g.R_START, 0));

			c.push_back(closure<0>(init_set, g));

			bool changed;
			do
			{
				changed = false;

				for(size_t i = 0; i < c.size(); i++)
				{
					auto i_set = c[i];
					
					for(stid x = 0; x < g.symbols_size(); x++)
					{
						auto goto_set = goto_f<0>(i_set, x, g);
						if(goto_set.size() == 0 || is_in(goto_set, c))
							continue;
						
						c.push_back(goto_set);
						changed = true;
					}
				}
			} while(changed);

			return c;
		}

		template <size_t L>
		static std::vector<item<L>> closure(const std::vector<item<L>> i_set, const grammar g) //dragon book, page 245
		{
			std::vector<item<L>> j_set(i_set);

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
				}
			} while(changed);

			return j_set;
		}
	};
}

#endif

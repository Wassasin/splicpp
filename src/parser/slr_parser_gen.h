#ifndef SLR_PARSER_GEN_H
#define SLR_PARSER_GEN_H

#include "../common/typedefs.h"

#include "ptable.h"
#include "item.h"

namespace splicpp
{
	class slr_parser_gen
	{
		slr_parser_gen() {}

	public:
		/*static ptable generate(const grammar g) //dragon book, page 253
		{
			
		}*/
	
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
		static bool is_in(const std::vector<item<L>> i_set, const std::vector<std::vector<item<L>>> c)
		{
			for(size_t i = 0; i < c.size(); i++)
			{
				if(i_set.size() != c.size())
					continue;
				
				bool all_in = true;
				for(size_t j = 0; j < i_set.size(); j++)
					if(!i_set[j].is_in(c[i]))
					{
						all_in = false;
						break;
					}

				if(all_in)
					return true;
			}

			return false;
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

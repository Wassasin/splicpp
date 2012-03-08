#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include "spl.hpp"
#include "ptable.hpp"
#include "clr_parser_gen.hpp"

namespace splicpp
{
	class spl_parser
	{
		const spl g;
		const ptable t;
		
	public:
		spl_parser()
		: g()
		, t(splicpp::clr_parser_gen::generate(g, resolve_conflicts))
		{
			std::cout << std::endl;
			t.print(g);
		}
		
		static void resolve_conflicts(std::vector<ptable::acttransition>& transitions, const size_t, const std::vector<itemset<1>>, const stid a, const grammar g)
		{
			/*
				Problem: S/R-conflict on rule
					nl_stmt :== l_if l_bracket_left nl_exp l_bracket_right nl_stmt
				
				To rule
					nl_stmt :== l_if l_bracket_left nl_exp l_bracket_right nl_stmt l_else nl_stmt
				
				Solution: choose shift if conflict on first rule
			*/
			
			if(transitions.size() != 2)
				return;
			
			if(a != g.fetch_stid("l_else"))
				return;
			
			size_t strans = 0, rtrans = 1;
			if(transitions.at(strans).t == ptable::acttransition::t_shift && transitions.at(rtrans).t == ptable::acttransition::t_shift)
				return; //shift-shift conflict
			
			if(transitions.at(strans).t == ptable::acttransition::t_reduce && transitions.at(rtrans).t == ptable::acttransition::t_shift)
				std::swap(strans, rtrans);
			
			assert(transitions.at(strans).t == ptable::acttransition::t_shift && transitions.at(rtrans).t == ptable::acttransition::t_reduce);
			
			const stid NL_STMT = g.fetch_stid("nl_stmt"), L_IF = g.fetch_stid("l_if"), L_BRACKET_LEFT = g.fetch_stid("l_bracket_left"), NL_EXP = g.fetch_stid("nl_exp"), L_BRACKET_RIGHT = g.fetch_stid("l_bracket_right");
			const rule r = rule(NL_STMT) + L_IF + L_BRACKET_LEFT + NL_EXP + L_BRACKET_RIGHT + NL_STMT;
			
			rule conflict_rule = g.fetch_rule(transitions.at(rtrans).rule);
			
			if(conflict_rule != r)
				return;
			
			transitions.erase(transitions.begin() + rtrans);
		}
	};
}

#endif

#ifndef PTABLE_H
#define PTABLE_H

#include <vector>

#include "../common/typedefs.hpp"
#include "../common/grammar.hpp"

#include "lexer.hpp"
#include "cst_element.hpp"

namespace splicpp
{
	class ptable
	{
	public:
		struct acttransition
		{
			enum acttranstype
			{
				t_error,
				t_accept,
				t_shift,
				t_reduce
			};
		
			acttranstype t;
			
			union {
				stateid state;
				rid rule;
			};
			
			static acttransition error()
			{
				acttransition x;
				x.t = acttranstype::t_error;
				return x;
			}
	
			static acttransition accept()
			{
				acttransition x;
				x.t = acttranstype::t_accept;
				return x;
			}
	
			static acttransition shift(stateid state)
			{
				acttransition x;
				x.t = acttranstype::t_shift;
				x.state = state;
				return x;
			}
	
			static acttransition reduce(rid rule)
			{
				acttransition x;
				x.t = acttranstype::t_reduce;
				x.rule = rule;
				return x;
			}
			
			void print() const;
		};
		
		struct gototransition
		{
			enum gototranstype
			{
				t_error,
				t_jump
			};
			
			gototranstype t;
			stateid state;
			
			static gototransition error()
			{
				gototransition x;
				x.t = gototranstype::t_error;
				return x;
			}
	
			static gototransition jump(stateid state)
			{
				gototransition x;
				x.t = gototranstype::t_jump;
				x.state = state;
				return x;
			}
			
			void print() const;
		};

	private:
		size_t terminals, nonterminals;

		std::vector<std::vector<acttransition>> acttable;
		std::vector<std::vector<gototransition>> gototable;

	public:
		ptable(size_t terminals, size_t nonterminals)
		: terminals(terminals)
		, nonterminals(nonterminals)
		, acttable()
		, gototable()
		{}
		
		void add_state(const std::vector<acttransition> actrow, const std::vector<gototransition> gotorow);
		cst_element parse(lexer& l) const;
		void print(const grammar g) const;
	};
}

#endif

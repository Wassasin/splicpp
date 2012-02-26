#ifndef PTABLE_H
#define PTABLE_H

#include <vector>
#include <stack>

#include "../common/typedefs.h"
#include "../common/grammar.h"

#include "lexer.h"

namespace splicpp
{
	class ptable
	{
	public:
		enum transtype
		{
			error,
			accept,
			shift,
			reduce
		};

		struct transition
		{
			transtype t;
			
			union {
				stateid state;
				rid rule;
			};
			
			static transition error()
			{
				transition x;
				x.t = transtype::error;
				return x;
			}
			
			static transition accept()
			{
				transition x;
				x.t = transtype::accept;
				return x;
			}
			
			static transition shift(stateid state)
			{
				transition x;
				x.t = transtype::shift;
				x.state = state;
				return x;
			}
			
			static transition reduce(rid rule)
			{
				transition x;
				x.t = transtype::reduce;
				x.rule = rule;
				return x;
			}	
		};

	private:
		size_t terminals, nonterminals;

		std::vector<std::vector<transition>> acttable;
		std::vector<std::vector<stid>> gototable;

	public:
		ptable(size_t terminals, size_t nonterminals)
		: terminals(terminals)
		, nonterminals(nonterminals)
		, acttable()
		, gototable()
		{}
		
		void add_state(const std::vector<transition> actrow, const std::vector<stid> gotorow)
		{
			if(actrow.size() != terminals || gotorow.size() != nonterminals)
				throw std::exception();
			
			acttable.push_back(actrow);
			gototable.push_back(gotorow);
		}

		std::vector<rid> parse(lexer& l) const
		{
			std::vector<rid> output;
			std::stack<stateid> stack;
			stack.push(0); //Push s0
		
			l.reset();
			
			token a = l.next();
			while(true)
			{
				stateid s = stack.top();
				stack.pop();
			
				transition t = acttable[s][a.type];
				if(t.t == shift)
				{
					stack.push(t.state);
					a = l.next();
				}
				else if(t.t == reduce)
				{
					rule r = l.get_grammar().fetch_rule(t.rule);
					for(uint i = 0; i < r.body.size(); i++)
						stack.pop();
					
					stateid st = stack.top();
					stack.push(gototable[st][r.start]);
					output.push_back(t.rule);
				}
				else if(t.t == accept)
				{
					break;
				}
				else
					throw std::exception();
			}
		}
	};
}

#endif

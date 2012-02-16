#ifndef PTABLE_H
#define PTABLE_H

#include <vector>
#include <stack>

#include "typedefs.h"
#include "grammar.h"
#include "../parser/lexer.h"

namespace splicpp
{
	class ptable
	{
		enum transtype
		{
			error,
			accept,
			shift,
			reduce
		};
		
		typedef uint stid; //State Identifier

		struct transition
		{
			transtype t;
			
			union {
				stid state;
				rid rule;
			};
		};

		uint terminals, nonterminals;

		std::vector<std::vector<transition>> acttable;
		std::vector<std::vector<stid>> gototable;

	public:
		ptable(uint terminals, uint nonterminals)
		: terminals(terminals)
		, nonterminals(nonterminals)
		, acttable()
		, gototable()
		{}
		
		std::vector<rid> parse(lexer& l)
		{
			std::vector<rid> output;
			std::stack<stid> stack;
			stack.push(0); //Push s0
		
			l.reset();
			
			token a = l.next();
			while(true)
			{
				stid s = stack.top();
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
					
					stid st = stack.top();
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

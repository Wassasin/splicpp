#ifndef PTABLE_H
#define PTABLE_H

#include <vector>
#include <stack>

#include "../common/typedefs.h"
#include "../common/grammar.h"

#include "lexer.h"
#include "cst.h"

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
			
			void print() const
			{
				switch(t)
				{
					case t_error:
						//std::cout << "err";
					break;
					case t_accept:
						std::cout << "acc";
					break;
					case t_shift:
						std::cout << "s" << state;
					break;
					case t_reduce:
						std::cout << "r" << rule;
					break;
				}
			}
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
			
			void print() const
			{
				switch(t)
				{
					case t_error:
						//std::cout << "err";
					break;
					case t_jump:
						std::cout << state;
					break;
				}
			}
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
		
		void add_state(const std::vector<acttransition> actrow, const std::vector<gototransition> gotorow)
		{
			if(actrow.size() != terminals || gotorow.size() != nonterminals)
			{
				std::cout << actrow.size() << std::endl;
				std::cout << terminals << std::endl;
				std::cout << gotorow.size() << std::endl;
				std::cout << nonterminals << std::endl;
				throw std::logic_error("parser table row does not have predetermined number of transitions");
			}
			
			acttable.push_back(actrow);
			gototable.push_back(gotorow);
		}
		
		cst_node::cst_element parse(lexer& l) const
		{
			const grammar g = l.get_grammar();
		
			std::stack<cst_node::cst_element> e_stack;
			std::stack<stateid> s_stack;
			
			l.reset();
			token a = l.next();
			
			s_stack.push(0); //Push s0
			while(true)
			{
				stateid s = s_stack.top();
			
				acttransition t = acttable.at(s).at(g.translate_lit(a.type));
				if(t.t == acttransition::t_shift)
				{
					s_stack.push(t.state);
					e_stack.push(cst_node::cst_element(a));
					
					a = l.next();
				}
				else if(t.t == acttransition::t_reduce)
				{
					rule r = g.fetch_rule(t.rule);
					
					std::shared_ptr<cst_node> n(new cst_node(t.rule));
					for(uint i = 0; i < r.body.size(); i++)
					{
						assert(!n->is_full(g));
						n->add_element(e_stack.top());
						
						s_stack.pop();
						e_stack.pop();
					}
					
					assert(n->is_full(g));
					
					stateid st = s_stack.top();
					gototransition gt = gototable.at(st).at(g.translate_nlit(r.start));
					
					if(gt.t == gototransition::t_error)
						throw std::exception();
					
					s_stack.push(gt.state);
					e_stack.push(cst_node::cst_element(n));
				}
				else if(t.t == acttransition::t_accept)
					break;
				else
					throw std::runtime_error("parser error on line "+a.line);
			}
			
			assert(e_stack.size() == 1);
			const auto result_el = e_stack.top();

			assert(result_el.is_node());
			const auto result_node = result_el.as_node();

			assert(result_node->fetch_stid(g) == g.NL_START);
			return result_node;
		}
		
		void print(const grammar g) const
		{
			std::cout << "\t|";
			
			for(size_t i = 0; i < g.symbols_size(); i++)
			{
				const auto s = g.fetch_symbol(i);
				if(s->type() == s_lit)
					std::cout << " " << s->name << "\t";
			}
			
			std::cout << "|";
			
			for(size_t i = 0; i < g.symbols_size(); i++)
			{
				const auto s = g.fetch_symbol(i);
				if(s->type() == s_nlit)
					std::cout << " " << s->name << "\t";
			}
			
			std::cout << std::endl << std::endl;
			
			for(size_t i = 0; i < acttable.size(); i++)
			{
				std::cout << i << "\t|";
				
				for(size_t j = 0; j < acttable[i].size(); j++)
				{
					acttable[i][j].print();
					std::cout << "\t";
				}
				
				std::cout << "|";
				
				for(size_t j = 0; j < gototable[i].size(); j++)
				{
					gototable[i][j].print();
					std::cout << "\t";
				}
				
				std::cout << std::endl;
			}
		}
	};
}

#endif

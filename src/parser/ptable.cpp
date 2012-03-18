#include "ptable.hpp"

#include <stack>
#include <sstream>

#include "../common/utils.hpp"

namespace splicpp
{
	void ptable::acttransition::print() const
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
			default:
				throw std::runtime_error("unexpected transitiontype");
		}
	}
	
	bool ptable::acttransition::operator==(const acttransition x) const
	{
		if(this->t != x.t)
			return false;
		
		if(this->t == acttranstype::t_shift)
			if(this->state != x.state)
				return false;
		
		if(this->t == acttranstype::t_reduce)
			if(this->rule != x.rule)
				return false;
		
		return true;
	}
	
	void ptable::gototransition::print() const
	{
		switch(t)
		{
			case t_error:
				//std::cout << "err";
			break;
			case t_jump:
				std::cout << state;
			break;
			default:
				throw std::runtime_error("unexpected transitiontype");
		}
	}

	void ptable::add_state(const std::vector<acttransition> actrow, const std::vector<gototransition> gotorow)
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
	
	cst_element ptable::parse(lexer& l) const
	{
		const grammar g = l.get_grammar();
	
		std::stack<std::shared_ptr<cst_element>> e_stack;
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
				e_stack.push(std::shared_ptr<cst_element>(new cst_element(a)));
				
				a = l.next();
			}
			else if(t.t == acttransition::t_reduce)
			{
				rule r = g.fetch_rule(t.rule);
				
				cst_node n(t.rule);
				for(uint i = 0; i < r.body.size(); i++)
				{
					assert(!n.is_full(g));
					n.add_element(e_stack.top());
					
					s_stack.pop();
					e_stack.pop();
				}
				assert(n.is_full(g));
				
				stateid st = s_stack.top();
				gototransition gt = gototable.at(st).at(g.translate_nlit(r.start));
				
				if(gt.t == gototransition::t_error)
					throw std::exception();
				
				s_stack.push(gt.state);
				e_stack.push(std::shared_ptr<cst_element>(new cst_element(n)));
			}
			else if(t.t == acttransition::t_accept)
				break;
			else
			{
				std::stringstream str;
				str << "parser error on line " << a.line+1 << " near \"" << a.as_string(l.get_str()) << "\" (" << g.fetch_symbol(a.type)->name << "), expected ";
				
				delim_end_printer p(", ", " or ", str);
				const auto ts = acttable.at(s);
				for(size_t i = 0; i < g.symbols_size(); i++)
				{
					const auto sym = g.fetch_symbol(i);
					if(sym->type() == s_lit && ts.at(g.translate_lit(i)).t != acttransition::t_error)
						p.print(sym->name);
				}
				
				p.finish();
				throw std::runtime_error(str.str());
			}
		}
		
		assert(e_stack.size() == 1);
		const auto result_el = e_stack.top();

		assert(result_el->is_node());
		const auto result_node = result_el->as_node();

		assert(result_node.fetch_stid(g) == g.NL_START);
		return result_node;
	}
	
	void ptable::print(const grammar g) const
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
}

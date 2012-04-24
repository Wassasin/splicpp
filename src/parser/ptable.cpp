#include "ptable.hpp"

#include <stack>
#include <sstream>
#include <boost/lexical_cast.hpp>

#include "../common/utils.hpp"

namespace splicpp
{
	std::string ptable::acttransition::str() const
	{
		std::stringstream ss;
	
		switch(t)
		{
			case t_error:
				//std::cout << "err";
			break;
			case t_accept:
				ss << "acc";
			break;
			case t_shift:
				ss << "s" << state;
			break;
			case t_reduce:
				ss << "r" << rule;
			break;
			default:
				throw std::runtime_error("unexpected transitiontype");
		}
		
		return ss.str();
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
	
	std::string ptable::gototransition::str() const
	{
		std::stringstream ss;
		
		switch(t)
		{
			case t_error:
				//std::cout << "err";
			break;
			case t_jump:
				ss << state;
			break;
			default:
				throw std::runtime_error("unexpected transitiontype");
		}
		
		return ss.str();
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
	
		std::stack<s_ptr<cst_element>> e_stack;
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
				e_stack.push(s_ptr<cst_element>(new cst_element(a)));
				
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
				e_stack.push(s_ptr<cst_element>(new cst_element(n)));
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
		const size_t num_size = 4;
		const size_t min_name_size = 4;
		
		print_spaces(num_size);
		std::cout << "|";
		
		std::vector<size_t> spacing;
		spacing.reserve(g.symbols_size());
		
		for(size_t i = 0; i < g.symbols_size(); i++)
		{
			const auto s = g.fetch_symbol(i);
			if(s->name.size() > min_name_size)
				spacing.push_back(s->name.size());
			else
				spacing.push_back(min_name_size);
		}
		
		for(size_t i = 0; i < g.symbols_size(); i++)
		{
			const auto s = g.fetch_symbol(i);
			if(s->type() == s_lit)
			{
				std::cout << ' ' << s->name;
				print_spaces(spacing.at(i) - s->name.size());
			}
		}
		
		std::cout << "|";
		
		for(size_t i = 0; i < g.symbols_size(); i++)
		{
			const auto s = g.fetch_symbol(i);
			if(s->type() == s_nlit)
			{
				std::cout << ' ' << s->name;
				print_spaces(spacing.at(i) - s->name.size());
			}
		}
		
		std::cout << std::endl << std::endl;
		
		for(size_t i = 0; i < acttable.size(); i++)
		{
			std::string i_str(boost::lexical_cast<std::string>(i));
			print_spaces(num_size - i_str.length());
			std::cout << i_str << "|";
			
			const auto blaat = gototable.at(i);
			for(size_t j = 0; j < acttable.at(i).size(); j++)
			{
				std::string str = acttable[i][j].str();
				std::cout << ' ' << str;
				print_spaces(spacing.at(g.rtranslate_lit(j)) - str.size());
			}
			
			std::cout << "|";
			
			for(size_t j = 0; j < gototable[i].size(); j++)
			{
				std::string str = gototable[i][j].str();
				std::cout << ' ' << str;
				print_spaces(spacing.at(g.rtranslate_nlit(j)) - str.size());
			}
			
			std::cout << std::endl;
		}
	}
	
	void ptable::print_spaces(const size_t n) const
	{
		for(size_t i = 0; i < n; i++)
			std::cout << ' ';
	}
}

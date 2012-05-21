#include "ir_temp_allocator.hpp"

#include <stdexcept>

#include "../common/generic.hpp"
#include "../common/udgraph.hpp"
#include "../common/graphcoloring.hpp"
#include "../ir/ircontext.hpp"

#include "ir_liveness_analyser.hpp"

#include "../ir/ir_exp_binop.hpp"
#include "../ir/ir_exp_const.hpp"
#include "../ir/ir_exp_eseq.hpp"
#include "../ir/ir_exp_mem.hpp"
#include "../ir/ir_exp_name.hpp"
#include "../ir/ir_exp_temp.hpp"

#include "../ir/ir_stmt_call.hpp"
#include "../ir/ir_stmt_cjump.hpp"
#include "../ir/ir_stmt_jump.hpp"
#include "../ir/ir_stmt_label.hpp"
#include "../ir/ir_stmt_move.hpp"
#include "../ir/ir_stmt_seq.hpp"

namespace splicpp
{
	void ir_temp_allocator::produce(s_ptr<const ir_exp> r)
	{
		acc = r;
	}
	
	s_ptr<const ir_exp> ir_temp_allocator::map(const s_ptr<const ir_exp> x)
	{
		x->map(*this);
		return acc;
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_stmt> x)
	{
		x->map(*this);
	}
	
	std::vector<s_ptr<const ir_stmt>> ir_temp_allocator::apply(const std::vector<s_ptr<const ir_stmt>>& stmts, const std::map<ir_temp, ir_temp>& reserved_temps, const std::vector<ir_temp>& scratch_temps, const ircontext& c)
	{
		udgraph<ir_temp> alloc_graph;
		
		//Add temporaries in a graph, with edges representing concurrent usage
		for(const auto& liveness : ir_liveness_analyser::analyse(stmts))
		{
			for(const ir_temp t : liveness.live_out)
				alloc_graph.add_vertex(t);
			
			for(const ir_temp ti : liveness.live_out)
				for(const ir_temp tj : liveness.live_out)
					if(ti != tj)
						alloc_graph.add_edge(ti, tj);
		}
		
		//Remove reserved temporaries from vertex
		for(std::pair<const ir_temp, const ir_temp> rt : reserved_temps)
			alloc_graph.remove_vertex(rt.first);
		
		const graphcoloring<ir_temp>::selected colored = graphcoloring<ir_temp>::color(alloc_graph, scratch_temps.size());

		std::map<ir_temp, s_ptr<const ir_exp>> tmap;
		for(const std::pair<const ir_temp, const ir_temp> rt : reserved_temps)
			tmap[rt.first] = make_s<ir_exp_temp>(rt.second);
		
		for(const std::pair<const ir_temp, const size_t> p : colored.map)
			tmap[p.first] = make_s<ir_exp_temp>(scratch_temps.at(p.second));
		
		for(const ir_temp t : colored.spilled)
			tmap[t] = make_s<ir_exp_mem>(make_s<ir_exp_name>(c.create_label()));
		
		ir_temp_allocator a(tmap);
		
		for(const auto stmt : stmts)
			stmt->map(a);
		
		return a.stmts;
	}

	//Inherited from ir_exp_transformer
	void ir_temp_allocator::map(const s_ptr<const ir_exp_binop> x)
	{
		produce(make_s<ir_exp_binop>(
			x->op,
			map(x->e_left),
			map(x->e_right)
		));
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_exp_const> x)
	{
		produce(x);
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_exp_eseq>)
	{
		throw std::logic_error("ir_exp_eseq is not allowed in this stage; run the ir_desequencer first");
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_exp_mem> x)
	{
		produce(make_s<ir_exp_mem>(map(x->e)));
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_exp_name> x)
	{
		produce(x);
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_exp_temp> x)
	{
		produce(tmap.at(x->t));
	}
	
	//Inherited from ir_stmt_transformer
	void ir_temp_allocator::map(const s_ptr<const ir_stmt_call>)
	{
		throw std::logic_error("ir_stmt_call is not allowed in this stage; run the ir_call_transformer first");
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_stmt_cjump> x)
	{
		stmts.push_back(make_s<ir_stmt_cjump>(
			x->r,
			map(x->e_left),
			map(x->e_right),
			x->l_left,
			x->l_right
		));
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_stmt_jump> x)
	{
		stmts.push_back(make_s<ir_stmt_jump>(
			map(x->e)
		));
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_stmt_label> x)
	{
		stmts.push_back(x);
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_stmt_move> x)
	{
		stmts.push_back(make_s<ir_stmt_move>(
			map(x->e_left),
			map(x->e_right)
		));
	}
	
	void ir_temp_allocator::map(const s_ptr<const ir_stmt_seq>)
	{
		throw std::logic_error("ir_stmt_seq is not allowed in this stage; run the ir_desequencer first");
	}
}

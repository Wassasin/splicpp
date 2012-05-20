#include "ir_liveness_analyser.hpp"

#include <stdexcept>
#include <queue>

#include "../common/generic.hpp"

#include "ir_cf_analyser.hpp"
#include "ir_label_analyser.hpp"
#include "ir_usage_analyser.hpp"

namespace splicpp
{
	std::vector<ir_liveness_analyser::liveness> ir_liveness_analyser::analyse(std::vector<s_ptr<const ir_stmt>> stmts)
	{
		dgraph<size_t> cf = ir_cf_analyser::analyse(
			stmts,
			ir_label_analyser::analyse(stmts)
		);
		
		std::vector<ir_usage_analyser::usage> usage = ir_usage_analyser::analyse(stmts);
		
		/*
			1. if v in use (n), then it is live-in at n
			2. if v is live-in at n, then it is live-out at all pred (n)
			3. if v is live-out at n and not in def (n), then it is live-in at n
		*/
		
		std::vector<liveness> liveness(stmts.size());
		std::unordered_set<size_t> to_check;
		
		//Rule 1: if v in use (n), then it is live-in at n
		for(size_t i = 0; i < liveness.size(); i++)
		{
			if(usage[i].used.empty())
				continue;
			
			to_check.insert(i);
			liveness[i].live_in.insert(
				usage[i].used.cbegin(),
				usage[i].used.cend()
			);
		}
		
		while(!to_check.empty())
		{
			const size_t n = *to_check.begin();
			to_check.erase(to_check.begin());
		
			//Rule 2: if v is live-in at n, then it is live-out at all pred (n)
			for(const size_t m : cf.pred(n))
				for(const ir_temp v : liveness[n].live_in)
				{
					liveness[m].live_out.insert(v);
					to_check.insert(m);
				}
			
			//Rule 3: if v is live-out at n and not in def (n), then it is live-in at n
			for(const ir_temp v : liveness[n].live_out)
				if(!usage[n].defined || usage[n].defined.get() != v)
				{
					liveness[n].live_in.insert(v);
					to_check.insert(n);
				}
		}

		return liveness;
	}
}

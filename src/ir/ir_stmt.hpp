#ifndef IR_STMT_H
#define IR_STMT_H

#include <ostream>
#include <vector>

#include "../common/typedefs.hpp"

namespace splicpp
{
	class ircontext;
	class ir_exp;

	class ir_stmt
	{
	public:
		ir_stmt()
		{}
	
		virtual void print(std::ostream& s, const uint tab) const = 0;
		
		virtual ~ir_stmt() {}
		
		static void cat(s_ptr<const ir_stmt>& r, const s_ptr<const ir_stmt> x);
		static s_ptr<const ir_stmt> push(const std::vector<s_ptr<const ir_exp>>& xs, const ircontext& c);
		static s_ptr<const ir_stmt> pop(const size_t count, const ircontext& c);
		
		static void print_newline(std::ostream& s, const uint tab)
		{
			s << std::endl;
			for(uint i = 0; i < tab; i++)
				s << "\t";
		}
	};
}

#endif

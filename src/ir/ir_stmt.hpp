#ifndef IR_STMT_H
#define IR_STMT_H

#include <ostream>
#include <vector>

#include "../common/typedefs.hpp"

#include "ir_exp_binop.hpp"

namespace splicpp
{
	class ir_stmt_mapper;
	class ircontext;
	class ir_exp;

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Weffc++"
	/*
		Ignore non-virtual-destructor warning
		This is a bug in GCC4.6
		See http://stackoverflow.com/questions/2571850/why-does-enable-shared-from-this-have-a-non-virtual-destructor
	*/
	class ir_stmt : public std::enable_shared_from_this<ir_stmt>
	{
	public:
		ir_stmt()
		{}
	
		virtual ~ir_stmt() {}
	
		virtual void map(ir_stmt_mapper& t) const = 0;
		virtual void print(std::ostream& s, const uint tab) const = 0;
		
		static void cat(s_ptr<const ir_stmt>& r, const s_ptr<const ir_stmt> x);
		static s_ptr<const ir_stmt> push(const std::vector<s_ptr<const ir_exp>>& xs, const ircontext& c);
		static s_ptr<const ir_stmt> pop(const size_t count, const ircontext& c);
		
		static s_ptr<const ir_exp> fetch_relative(const int i, const ir_temp t, const ir_exp_binop::binop op = ir_exp_binop::op_plus);
		
		static void print_newline(std::ostream& s, const uint tab)
		{
			s << std::endl;
			for(uint i = 0; i < tab; i++)
				s << "\t";
		}
	};
	#pragma GCC diagnostic pop
}

#endif

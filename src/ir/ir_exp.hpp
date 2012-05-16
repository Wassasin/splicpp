#ifndef IR_EXP_H
#define IR_EXP_H

#include <ostream>
#include <memory>

namespace splicpp
{
	class ir_exp_mapper;
	
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Weffc++"
	/*
		Ignore non-virtual-destructor warning
		This is a bug in GCC4.6
		See http://stackoverflow.com/questions/2571850/why-does-enable-shared-from-this-have-a-non-virtual-destructor
	*/
	class ir_exp : public std::enable_shared_from_this<ir_exp>
	{
	public:
		ir_exp()
		{}
		
		virtual ~ir_exp() {}
		
		virtual void map(ir_exp_mapper& t) const = 0;
		virtual void print(std::ostream& s, const uint tab) const = 0;
		
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

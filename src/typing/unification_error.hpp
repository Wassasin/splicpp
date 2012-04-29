#ifndef UNIFICATION_ERROR_H
#define UNIFICATION_ERROR_H

#include <stdexcept>
#include <memory>
#include <sstream>

#include "types/sl_type.hpp"

namespace splicpp
{
	class unification_error : public std::runtime_error
	{
		const s_ptr<const sl_type> t1, t2;
	
		static std::string create_message(const s_ptr<const sl_type> t1, const s_ptr<const sl_type> t2)
		{
			std::stringstream s;
			s << "Cannot unify types ";
			t1->print(s);
			s << " and ";
			t2->print(s);
			return s.str();
		}
	
	public:
		unification_error(const s_ptr<const sl_type> t1, const s_ptr<const sl_type> t2)
		: runtime_error(create_message(t1, t2))
		, t1(t1)
		, t2(t2)
		{}
		
		void print(const std::string str, std::ostream& s)
		{
			s << "Cannot unify types:";
			
			s << std::endl << "Type ";
			t1->print(s);
			s << ", line " << t1->sl.line+1 << ':' << std::endl;
			t1->sl.print(str, s);
			
			s << std::endl << "Type ";
			t2->print(s);
			s << ", line " << t2->sl.line+1 << ':' << std::endl;
			t2->sl.print(str, s);
		}
		
		~unification_error() throw() {}
	};
}

#endif

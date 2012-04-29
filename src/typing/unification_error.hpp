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
		const s_ptr<const sl_type> t1, t2, u1, u2;
	
		static std::string create_message(const s_ptr<const sl_type> t1, const s_ptr<const sl_type> t2, const s_ptr<const sl_type> u1, const s_ptr<const sl_type> u2)
		{
			std::stringstream s;
			s << "Cannot unify types ";
			t1->print(s);
			s << " and ";
			t2->print(s);
			s << " (specifically ";
			u1->print(s);
			s << " and ";
			u2->print(s);
			s << ")";
			
			return s.str();
		}
	
	public:
		unification_error(const s_ptr<const sl_type> t1, const s_ptr<const sl_type> t2, const s_ptr<const sl_type> u1, const s_ptr<const sl_type> u2)
		: runtime_error(create_message(t1, t2, u1, u2))
		, t1(t1)
		, t2(t2)
		, u1(u1)
		, u2(u2)
		{}
		
		void print(const std::string str, std::ostream& s)
		{
			s << "Cannot unify types:" << std::endl;
			
			s << std::endl;
			u1->print(s);
			s << " in ";
			t1->print(s);
			s << " [line " << u1->sl.line << ", pos " << u1->sl.pos << "]" << std::endl;			
			u1->sl.print(str, s);
			
			s << std::endl;
			
			s << std::endl;
			u2->print(s);
			s << " in ";
			t2->print(s);
			s << " [line " << u2->sl.line << ", pos " << u2->sl.pos << "]" << std::endl;			
			u2->sl.print(str, s);
		}
		
		~unification_error() throw() {}
	};
}

#endif

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
		const cs_ptr<sl_type> t1, t2;
	
		static std::string create_message(const cs_ptr<sl_type> t1, const cs_ptr<sl_type> t2)
		{
			std::stringstream s;
			s << "Cannot unify types ";
			t1->print(s);
			s << " and ";
			t2->print(s);
			return s.str();
		}
	
	public:
		unification_error(const cs_ptr<sl_type> t1, const cs_ptr<sl_type> t2)
		: runtime_error(create_message(t1, t2))
		, t1(t1)
		, t2(t2)
		{}
	};
}

#endif

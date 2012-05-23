#ifndef SSM_INSTRUCTIONS_H
#define SSM_INSTRUCTIONS_H

#include <ostream>
#include <boost/optional.hpp>

#include "../../common/typedefs.hpp"

namespace splicpp
{
	struct ssm
	{
		virtual int cost() const = 0;
		virtual void print(std::ostream& o) const = 0;
	};
	
	struct ssm_add : public ssm
	{
		virtual int cost() const
		{
			return 1;
		}
		
		virtual void print(std::ostream& o) const
		{
			o << "add";
		}
	};
	
	struct ssm_ajs : public ssm
	{
		const int amount;
	
		ssm_ajs(const int amount)
		: amount(amount)
		{}
	};
	
	struct ssm_nop : public ssm
	{
		virtual int cost() const
		{
			return 1;
		}
		
		virtual void print(std::ostream& o) const
		{
			o << "nop";
		}
	};
}

#endif

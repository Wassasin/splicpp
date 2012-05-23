#ifndef SSM_TRESULT_H
#define SSM_TRESULT_H

#include <list>
#include <vector>
#include <boost/optional.hpp>

#include "../../common/typedefs.hpp"

namespace splicpp
{
	struct ssm;

	class ssm_tresult
	{
		struct ssm_tresult_internal
		{
			std::list<s_ptr<const ssm>> data;
			bool destroyed;
			
			ssm_tresult_internal()
			: data()
			, destroyed(false)
			{}
		};
	
		s_ptr<ssm_tresult_internal> instructions;
		int cost;
		
	public:	
		ssm_tresult()
		: instructions(new ssm_tresult_internal())
		, cost(0)
		{}
		
		void append(const s_ptr<const ssm> instruction);
		void append(const ssm_tresult& other);
		bool destroyed() const;
		int fetch_cost() const;
		std::list<s_ptr<const ssm>> fetch_instructions() const;
	};
	
	typedef boost::optional<ssm_tresult> ssm_tresult_opt;
}

#endif

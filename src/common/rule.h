#ifndef RULE_H
#define RULE_H

#include <vector>

#include "typedefs.h"

namespace splicpp
{
	struct rule
	{
		stid start;
		std::vector<stid> body;
		
		rule(stid start)
		: start(start)
		, body()
		{}
		
		void append_body(const stid s)
		{
			body.push_back(s);
		}
		
		rule operator+(stid s)
		{
			append_body(s);
			return *this;
		}
		
		bool is_epsilon()
		{
			return body.empty();
		}
		
		void print()
		{
			std::cout << start << " :== ";
				
			for(uint i = 0; i < body.size(); i++)
			{
				if(i > 0)
					std::cout << ' ';
					
				std::cout << body[i];
			}
			
			std::cout << std::endl;
		}
	};
}

#endif



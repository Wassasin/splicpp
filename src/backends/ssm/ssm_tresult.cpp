#include "ssm_tresult.hpp"

#include <stdexcept>

#include "ssm_instructions.hpp"

namespace splicpp
{
	void ssm_tresult::append(const s_ptr<const ssm> instruction)
	{
		if(destroyed())
			throw std::runtime_error("Can not append to destroyed object");
	
		instructions->data.push_back(instruction);
		cost += instruction->cost();
	}
	
	void ssm_tresult::append(const ssm_tresult& other)
	{
		if(destroyed())
			throw std::runtime_error("Can not append to destroyed object");
		
		if(other.destroyed())
			throw std::runtime_error("Can not append a destroyed object");
	
		instructions->data.splice(instructions->data.end(), other.instructions->data);
		other.instructions->destroyed = true;
		cost += other.cost;
	}
	
	bool ssm_tresult::destroyed() const
	{
		return instructions->destroyed;
	}
	
	int ssm_tresult::fetch_cost() const
	{
		return cost;
	}
	
	std::list<s_ptr<const ssm>> ssm_tresult::fetch_instructions() const
	{
		return instructions->data;
	}
}

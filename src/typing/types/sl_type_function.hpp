#ifndef SL_TYPE_FUNCTION_H
#define SL_TYPE_FUNCTION_H

#include <vector>
#include <memory>

#include "sl_type.hpp"

namespace splicpp
{	
	class sl_type_function : public sl_type
	{
		std::vector<std::shared_ptr<sl_type>> args;
		std::shared_ptr<sl_type> r;
	public:
		sl_type_function(const std::vector<std::shared_ptr<sl_type>> args, const std::shared_ptr<sl_type> r)
		: args(args)
		, r(r)
		{}
		
		virtual sl_type_type type() const;
		virtual void print(std::ostream& s) const;
		
		virtual std::vector<std::shared_ptr<sl_type_unbound>> fv() const;
		//virtual substitution unify(const std::shared_ptr<sl_type> t, typecontext& c) const; //TODO
		virtual std::shared_ptr<sl_type> apply(const substitution& s) const;
	};
}

#endif
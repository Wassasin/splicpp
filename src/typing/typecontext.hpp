#ifndef TYPECONTEXT_H
#define TYPECONTEXT_H

#include <vector>
#include <memory>
#include <boost/optional.hpp>

#include "../common/typedefs.hpp"

namespace splicpp
{
	class sl_type;
	class sl_type_unbound;
	class substitution;

	class typecontext
	{
		std::vector<boost::optional<std::shared_ptr<sl_type>>> types;
		std::shared_ptr<uint> ft_count;
		
	public:
		typecontext()
		: types()
		, ft_count(new uint(0))
		{}
		
		void register_type(const sid i, const std::shared_ptr<sl_type> t);
		bool is_registered(const sid i) const;
		std::shared_ptr<sl_type> operator[](const sid i) const;
		std::shared_ptr<sl_type_unbound> create_fresh() const;
		typecontext apply(const substitution& s) const;
		std::vector<std::shared_ptr<sl_type_unbound>> fv() const;
		void print(std::ostream& s) const;
		void print_debug() const;
	};
}

#endif

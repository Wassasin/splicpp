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
	class sl_polytype;
	class substitution;
	class ltypecontext;

	class typecontext
	{
	protected:
		std::vector<boost::optional<std::shared_ptr<sl_polytype>>> types;
		std::shared_ptr<uint> ft_count;
		
		typecontext(decltype(ft_count) ft_count)
		: types()
		, ft_count(ft_count)
		{}
		
		typecontext(decltype(types) types, decltype(ft_count) ft_count)
		: types(types)
		, ft_count(ft_count)
		{}
		
	public:
		typecontext()
		: types()
		, ft_count(new uint(0))
		{}
		
		virtual ~typecontext() {}
		
		virtual void register_type(const sid i, const std::shared_ptr<sl_polytype> t);
		
		bool is_registered(const sid i) const;
		std::shared_ptr<sl_polytype> operator[](const sid i) const;
		std::shared_ptr<sl_type_unbound> create_fresh() const;
		typecontext apply(const substitution& s) const;
		typecontext apply_maintain(const substitution& s) const;
		std::vector<std::shared_ptr<sl_type_unbound>> fv() const;
		void print(std::ostream& s) const;
		void print_debug() const;
		
		friend class ltypecontext; //Required to access ft_count
	};
}

#endif

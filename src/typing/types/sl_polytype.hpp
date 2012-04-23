#ifndef SL_POLYTYPE_H
#define SL_POLYTYPE_H

#include "sl_type.hpp"

namespace splicpp
{
	class sl_type;
	class sl_type_unbound;
	class substitution;
	
	class sl_polytype
	{
	public:
		virtual ~sl_polytype() {}
		
		virtual std::shared_ptr<sl_type> unbind(const typecontext& c) const = 0;
		
		virtual std::vector<std::shared_ptr<sl_type_unbound>> tv() const = 0;
		virtual std::shared_ptr<sl_polytype> apply(const typecontext& c, const substitution& s) const = 0;
		
		virtual void print(std::ostream& s) const = 0;
		
		static std::shared_ptr<sl_polytype> qualify(const typecontext& c, const std::shared_ptr<sl_type> t);
		static std::shared_ptr<sl_polytype> not_qualify(const std::shared_ptr<sl_type> t);
	};
	
	class sl_polytype_forall : public sl_polytype
	{
		std::shared_ptr<sl_type> t;
		std::vector<std::shared_ptr<sl_type_unbound>> bindings;
		
		void bind(std::shared_ptr<sl_type_unbound> t);
		
	public:
		sl_polytype_forall(const std::shared_ptr<sl_type> t)
		: t(t)
		, bindings()
		{}
		
		std::shared_ptr<sl_type> unbind_maintain() const;
		
		virtual std::shared_ptr<sl_type> unbind(const typecontext& c) const;
		
		virtual std::vector<std::shared_ptr<sl_type_unbound>> tv() const;
		virtual std::shared_ptr<sl_polytype> apply(const typecontext& c, const substitution& s) const;
		
		virtual void print(std::ostream& s) const;
		
		static std::shared_ptr<sl_polytype_forall> qualify(const typecontext& c, const std::shared_ptr<sl_type> t);
		static std::shared_ptr<sl_polytype_forall> not_qualify(const std::shared_ptr<sl_type> t);
	};
	
	class sl_polytype_exists : public sl_polytype
	{
		std::vector<std::shared_ptr<sl_type_unbound>> bindings;
	
	public:
		sl_polytype_exists()
		: bindings()
		{}
		
		virtual std::shared_ptr<sl_type> unbind(const typecontext& c) const;
		
		virtual std::vector<std::shared_ptr<sl_type_unbound>> tv() const;
		virtual std::shared_ptr<sl_polytype> apply(const typecontext& c, const substitution& s) const;
		
		virtual void print(std::ostream& s) const;
	};
}

#endif

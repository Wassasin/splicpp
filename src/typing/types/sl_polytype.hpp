#ifndef SL_POLYTYPE_H
#define SL_POLYTYPE_H

#include "sl_type.hpp"

namespace splicpp
{
	class sl_type;
	class sl_type_unbound;
	class substitution;
	
	class sl_polytype : public std::enable_shared_from_this<sl_polytype>
	{
	public:
		virtual ~sl_polytype() {}
		
		virtual s_ptr<sl_type> unbind(const typecontext& c) const = 0;
		
		virtual std::vector<s_ptr<sl_type_unbound>> tv() const = 0;
		virtual s_ptr<sl_polytype> apply(const typecontext& c, const substitution& s) const = 0;
		
		virtual void print(std::ostream& s) const = 0;
		
		static s_ptr<sl_polytype> qualify(const typecontext& c, const s_ptr<sl_type> t);
		static s_ptr<sl_polytype> not_qualify(const s_ptr<sl_type> t);
	};
	
	class sl_polytype_forall : public sl_polytype
	{
		s_ptr<sl_type> t;
		std::vector<s_ptr<sl_type_unbound>> bindings;
		
		void bind(s_ptr<sl_type_unbound> t);
		
	public:
		sl_polytype_forall(const s_ptr<sl_type> t)
		: t(t)
		, bindings()
		{}
		
		s_ptr<sl_type> unbind_maintain() const;
		
		virtual s_ptr<sl_type> unbind(const typecontext& c) const;
		
		virtual std::vector<s_ptr<sl_type_unbound>> tv() const;
		virtual s_ptr<sl_polytype> apply(const typecontext& c, const substitution& s) const;
		
		virtual void print(std::ostream& s) const;
		
		static s_ptr<sl_polytype_forall> qualify(const typecontext& c, const s_ptr<sl_type> t);
		static s_ptr<sl_polytype_forall> not_qualify(const s_ptr<sl_type> t);
	};
	
	class sl_polytype_exists : public sl_polytype
	{
		std::vector<s_ptr<sl_type_unbound>> bindings;
	
	public:
		sl_polytype_exists()
		: bindings()
		{}
		
		virtual s_ptr<sl_type> unbind(const typecontext& c) const;
		
		virtual std::vector<s_ptr<sl_type_unbound>> tv() const;
		virtual s_ptr<sl_polytype> apply(const typecontext& c, const substitution& s) const;
		
		virtual void print(std::ostream& s) const;
	};
}

#endif

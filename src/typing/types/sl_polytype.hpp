#ifndef SL_POLYTYPE_H
#define SL_POLYTYPE_H

#include "sl_type.hpp"

namespace splicpp
{
	class sl_type;
	class sl_type_unbound;
	class substitution;
	
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Weffc++"
	/*
		Ignore non-virtual-destructor warning
		This is a bug in GCC4.6
		See http://stackoverflow.com/questions/2571850/why-does-enable-shared-from-this-have-a-non-virtual-destructor
	*/
	class sl_polytype : public std::enable_shared_from_this<sl_polytype>
	{
	public:
		virtual ~sl_polytype() {}
		
		virtual s_ptr<const sl_type> unbind(const typecontext& c) const = 0;
		
		virtual std::vector<s_ptr<const sl_type_unbound>> tv() const = 0;
		virtual s_ptr<const sl_polytype> apply(const typecontext& c, const substitution& s) const = 0;
		
		virtual void print(std::ostream& s) const = 0;
		
		static s_ptr<const sl_polytype> qualify(const typecontext& c, const s_ptr<const sl_type> t);
		static s_ptr<const sl_polytype> not_qualify(const s_ptr<const sl_type> t);
	};
	#pragma GCC diagnostic pop
	
	class sl_polytype_forall : public sl_polytype
	{
		s_ptr<const sl_type> t;
		std::vector<s_ptr<const sl_type_unbound>> bindings;
		
		void bind(s_ptr<const sl_type_unbound> t);
		
	public:
		sl_polytype_forall(const s_ptr<const sl_type> t)
		: t(t)
		, bindings()
		{}
		
		s_ptr<const sl_type> unbind_maintain() const;
		
		virtual s_ptr<const sl_type> unbind(const typecontext& c) const;
		
		virtual std::vector<s_ptr<const sl_type_unbound>> tv() const;
		virtual s_ptr<const sl_polytype> apply(const typecontext& c, const substitution& s) const;
		
		virtual void print(std::ostream& s) const;
		
		static s_ptr<const sl_polytype_forall> qualify(const typecontext& c, const s_ptr<const sl_type> t);
		static s_ptr<const sl_polytype_forall> not_qualify(const s_ptr<const sl_type> t);
	};
	
	class sl_polytype_exists : public sl_polytype
	{
		s_ptr<std::vector<s_ptr<const sl_type_unbound>>> bindings;
	
	public:
		sl_polytype_exists()
		: bindings(new std::vector<s_ptr<const sl_type_unbound>>())
		{}
		
		substitution propagate_findings(const typecontext& c, const s_ptr<const sl_polytype> t, substitution s) const;
		std::vector<s_ptr<const sl_type_unbound>> fetch_bindings() const;
		
		virtual s_ptr<const sl_type> unbind(const typecontext& c) const;
		
		virtual std::vector<s_ptr<const sl_type_unbound>> tv() const;
		virtual s_ptr<const sl_polytype> apply(const typecontext& c, const substitution& s) const;
		
		virtual void print(std::ostream& s) const;
	};
}

#endif

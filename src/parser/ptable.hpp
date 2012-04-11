#ifndef PTABLE_H
#define PTABLE_H

#include <vector>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

#include "../common/typedefs.hpp"
#include "../common/grammar.hpp"

#include "lexer.hpp"
#include "cst_element.hpp"

namespace splicpp
{
	class ptable
	{
	public:
		class acttransition
		{
		public:
			enum acttranstype
			{
				t_error,
				t_accept,
				t_shift,
				t_reduce
			};
		
			acttranstype t;
			
			union {
				stateid state;
				rid rule;
			};
			
		private:
			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive & ar, const unsigned int)
			{
				ar & t;
				ar & state;
			}
		
		protected:
			acttransition()
			: t()
			, state()
			{}
		
			acttransition(acttranstype t, stateid state)
			: t(t)
			, state(state)
			{}
		
		public:
			static acttransition error()
			{
				acttransition x;
				x.t = acttranstype::t_error;
				return x;
			}
	
			static acttransition accept()
			{
				acttransition x;
				x.t = acttranstype::t_accept;
				return x;
			}
	
			static acttransition shift(stateid state)
			{
				acttransition x;
				x.t = acttranstype::t_shift;
				x.state = state;
				return x;
			}
	
			static acttransition reduce(rid rule)
			{
				acttransition x;
				x.t = acttranstype::t_reduce;
				x.rule = rule;
				return x;
			}
			
			std::string str() const;
			bool operator==(const acttransition x) const;
		};
		
		struct gototransition
		{
			enum gototranstype
			{
				t_error,
				t_jump
			};
			
			gototranstype t;
			stateid state;
			
			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive & ar, const unsigned int)
			{
				ar & t;
				ar & state;
			}
			
			static gototransition error()
			{
				gototransition x;
				x.t = gototranstype::t_error;
				return x;
			}
	
			static gototransition jump(stateid state)
			{
				gototransition x;
				x.t = gototranstype::t_jump;
				x.state = state;
				return x;
			}
			
			std::string str() const;
		};

	private:
		size_t terminals, nonterminals;

		std::vector<std::vector<acttransition>> acttable;
		std::vector<std::vector<gototransition>> gototable;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int)
		{
		    ar & terminals;
		    ar & nonterminals;
		    ar & acttable;
		    ar & gototable;
		}
		
		void print_spaces(const size_t n) const;

	public:
		ptable()
		: terminals(0)
		, nonterminals()
		, acttable()
		, gototable()
		{}
	
		ptable(size_t terminals, size_t nonterminals)
		: terminals(terminals)
		, nonterminals(nonterminals)
		, acttable()
		, gototable()
		{}
		
		void add_state(const std::vector<acttransition> actrow, const std::vector<gototransition> gotorow);
		cst_element parse(lexer& l) const;
		void print(const grammar g) const;
	};
}

#endif

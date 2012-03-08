#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <boost/optional.hpp>
#include <boost/regex.hpp>

#include <string>

#include "typedefs.hpp"

namespace splicpp
{
	enum stype
	{
		s_lit,
		s_nlit,
		s_epsilon,
		s_ignored
	};

	class symbol
	{
	public:
		std::string name;

		symbol(const std::string name)
		: name(name)
		{}
		
		virtual stype type() const = 0;
	
		virtual ~symbol() {}
	};
	
	class epsilon : public symbol
	{
	public:
		epsilon()
		: symbol("epsilon")
		{}
			
		virtual stype type() const
		{
			return s_epsilon;
		}
		
		virtual boost::optional<uint> match(const std::string, const uint) const
		{
			return boost::optional<uint>();
		}
	};

	class literal : public symbol
	{		
	public:
		literal(const std::string name)
		: symbol(name)
		{}

		virtual stype type() const
		{
			return s_lit;
		}

		virtual boost::optional<uint> match(const std::string source, const uint pos) const = 0;
	};

	class static_literal : public literal
	{
		std::string str;
	public:
		static_literal(const std::string name, const std::string str)
		: literal(name)
		, str(str)
		{}

		virtual boost::optional<uint> match(const std::string source, const uint pos) const;
	};

	class regex_literal : public literal
	{
		boost::regex expr;

	public:
		regex_literal(const std::string name, const std::string expr)
		: literal(name)
		, expr(std::string("^").append(expr).append(".*$"), boost::regex::perl)
		{}
		
		virtual boost::optional<uint> match(const std::string source, const uint pos) const;
	};

	class end_literal : public literal
	{
	public:
		end_literal()
		: literal("$")
		{}
		
		virtual boost::optional<uint> match(const std::string source, const uint pos) const;
	};

	class non_literal : public symbol
	{
	public:
		non_literal(const std::string name)
		: symbol(name)
		{}
		
		virtual stype type() const
		{
			return s_nlit;
		}
	};
	
	class ignored_literal : public literal
	{
	public:
		ignored_literal(const std::string name)
		: literal(name)
		{}
		
		virtual stype type() const
		{
			return s_lit;
		}
	};
	
	class block_comment_literal : public ignored_literal
	{
		const std::string start, end;
	public:
		block_comment_literal(const std::string name, const std::string start, const std::string end)
		: ignored_literal(name)
		, start(start)
		, end(end)
		{}
	
		virtual boost::optional<uint> match(const std::string source, const uint pos) const;
	};
	
	class line_comment_literal : public block_comment_literal
	{
	public:
		line_comment_literal(const std::string name, const std::string str)
		: block_comment_literal(name, str, "\n")
		{}
	};
}

#endif




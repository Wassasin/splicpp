#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <memory>

#include "symbols.hpp"
#include "rule.hpp"
#include "token.hpp"
#include "typedefs.hpp"

namespace splicpp
{
	class grammar
	{
		std::vector<std::shared_ptr<symbol>> symbols;
		std::vector<rule> rules;
		
		std::vector<char> ignore_list;

	public:
		const stid NL_REAL_START, NL_START, L_END, S_EPSILON;
		const rid R_START;

		grammar()
		: symbols()
		, rules()
		, ignore_list()

		, NL_REAL_START(add_symbol(new non_literal("S'")))
		, NL_START(add_symbol(new non_literal("S")))
		, L_END(add_symbol(new end_literal()))
		, S_EPSILON(add_symbol(new epsilon()))

		, R_START(add_rule(rule(NL_REAL_START) + NL_START))
		{}
		
		virtual ~grammar() {}

		void ignore(const char c);
		bool should_ignore(const char c) const;
		stid add_symbol(symbol* s);
		rid add_rule(rule r);
		
		void remove_epsilons(rule& r);
		
		boost::optional<token> try_match(const std::string source, const uint pos, const uint line) const;
		
		void print() const;
		void print_token(const token t, const std::string source) const;
		void print_rule(const rid r) const;
		void print_symbol(const stid i) const;
		
		const rule fetch_rule(const rid i) const;
		stid fetch_stid(const std::string name) const;
		std::shared_ptr<symbol> fetch_symbol(const stid i) const;
		std::shared_ptr<symbol> fetch_symbol(const std::string name) const;
		
		size_t rules_size() const;
		size_t symbols_size() const;
		size_t nterminals_size() const;
		size_t terminals_size() const;
		
		size_t translate_lit(const stid id) const;
		size_t translate_nlit(const stid id) const;
	};
}

#endif

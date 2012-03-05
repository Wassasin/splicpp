#ifndef SLR_TEST0_H
#define SLR_TEST0_H

#include "../../common/typedefs.h"
#include "../../common/symbols.h"
#include "../../common/grammar.h"

namespace splicpp
{
	class slr_test0 : public grammar
	{
		const stid NL_E, NL_T, NL_F, L_ID, L_PLUS, L_TIMES, L_LBRACKET, L_RBRACKET;
		
		public:
			slr_test0()
			: grammar()

			, NL_E(add_symbol(		new non_literal("E")))
			, NL_T(add_symbol(		new non_literal("T")))
			, NL_F(add_symbol(		new non_literal("F")))
			
			, L_ID(add_symbol(		new regex_literal("id", "([a-zA-Z0-9\\-]+)")))
			, L_PLUS(add_symbol(	new static_literal("+", "+")))
			, L_TIMES(add_symbol(	new static_literal("*", "*")))
			, L_LBRACKET(add_symbol(new static_literal("(", "(")))
			, L_RBRACKET(add_symbol(new static_literal(")", ")")))
			{
				add_rule(rule(NL_START) + NL_E);
				
				add_rule(rule(NL_E) + NL_E + L_PLUS + NL_T);
				add_rule(rule(NL_E) + NL_T);
				add_rule(rule(NL_T) + NL_T + L_TIMES + NL_F);
				add_rule(rule(NL_T) + NL_F);
				add_rule(rule(NL_F) + L_LBRACKET + NL_E + L_RBRACKET);
				add_rule(rule(NL_F) + L_ID);
				
				ignore(' ');
				ignore('\t');
			}
	};
}

#endif

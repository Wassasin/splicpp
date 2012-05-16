#include "ast_construct.hpp"

#include <vector>

#include "../common/generic.hpp"

#include "../typing/typecontext.hpp"
#include "../typing/types/sl_polytype.hpp"
#include "../typing/types/sl_type_array.hpp"
#include "../typing/types/sl_type_bool.hpp"
#include "../typing/types/sl_type_function.hpp"
#include "../typing/types/sl_type_tuple.hpp"
#include "../typing/types/sl_type_unbound.hpp"
#include "../typing/types/sl_type_void.hpp"

#include "../ir/ircontext.hpp"
#include "../ir/ir_exp_binop.hpp"
#include "../ir/ir_exp_const.hpp"
#include "../ir/ir_exp_mem.hpp"
#include "../ir/ir_exp_name.hpp"
#include "../ir/ir_exp_temp.hpp"
#include "../ir/ir_stmt_cjump.hpp"
#include "../ir/ir_stmt_jump.hpp"
#include "../ir/ir_stmt_label.hpp"


namespace splicpp
{
	void ast_construct::pretty_print(std::ostream& s, const uint) const
	{
		s << "__INTERNAL__";
	}
	
	void ast_construct::assign(const sid i)
	{
		id = i;
	}
	
	sid ast_construct::fetch_id() const
	{
		return id.get();
	}
	
	substitution ast_construct::declare_type(typecontext& c) const
	{
		c.register_type(id.get(), sl_polytype::qualify(c, fetch_type(c)));
		return substitution::id();
	}
	
	/* print, isEmpty, head, tail, fst, and snd */
	
	std::string ast_construct_print::fetch_name() const
	{
		return "print";
	}
	
	s_ptr<const sl_type> ast_construct_print::fetch_type(const typecontext& c) const
	{
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			std::static_pointer_cast<const sl_type>(c.create_fresh(sl))
		));
		
		return make_s<sl_type_function>(args, make_s<sl_type_void>(sl), sl);
	}
	
	s_ptr<const ir_stmt> ast_construct_print::translate(const ir_label l_construct, const ircontext& c) const
	{
		s_ptr<const ir_stmt> r(make_s<ir_stmt_label>(l_construct));
		
		//TODO - No IR-code available to interact with console
		
		ir_stmt::cat(r, make_s<ir_stmt_jump>(
			make_s<ir_exp_mem>(
				make_s<ir_exp_binop>(
					ir_exp_binop::op_plus,
					make_s<ir_exp_temp>(c.frame_reg),
					make_s<ir_exp_const>(1)
				)
			)
		));
		
		return r;
	}
	
	std::string ast_construct_is_empty::fetch_name() const
	{
		return "isEmpty";
	}
	
	s_ptr<const sl_type> ast_construct_is_empty::fetch_type(const typecontext& c) const
	{
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			make_s<sl_type_array>(c.create_fresh(sl), sl)
		));
		
		return make_s<sl_type_function>(args, make_s<sl_type_bool>(sl), sl);
	}
	
	s_ptr<const ir_stmt> ast_construct_is_empty::translate(const ir_label l_construct, const ircontext& c) const
	{
		ir_label l_true = c.create_label();
		ir_label l_false = c.create_label();
		
		const s_ptr<const ir_exp> e_return(make_s<ir_exp_mem>(
			make_s<ir_exp_binop>(
				ir_exp_binop::op_plus,
				make_s<ir_exp_temp>(c.frame_reg),
				make_s<ir_exp_const>(1)
			)
		));
	
		s_ptr<const ir_stmt> r(make_s<ir_stmt_label>(l_construct));
		
		ir_stmt::cat(r, make_s<ir_stmt_cjump>(
			ir_stmt_cjump::op_eq,
			make_s<ir_exp_mem>(make_s<ir_exp_binop>(
					ir_exp_binop::op_plus,
					make_s<ir_exp_temp>(c.frame_reg),
					make_s<ir_exp_const>(2) //First argument on stack
			)),
			make_s<ir_exp_name>(c.l_nil),
			l_true,
			l_false
		));
				
		ir_stmt::cat(r, make_s<ir_stmt_label>(l_true));
		ir_stmt::cat(r, ir_stmt::push(create_vector_ptr<const ir_exp>(make_s<ir_exp_const>(true)), c));
		ir_stmt::cat(r, make_s<ir_stmt_jump>(e_return));
		
		ir_stmt::cat(r, make_s<ir_stmt_label>(l_false));
		ir_stmt::cat(r, ir_stmt::push(create_vector_ptr<const ir_exp>(make_s<ir_exp_const>(false)), c));
		ir_stmt::cat(r, make_s<ir_stmt_jump>(e_return));
		
		return r;
	}
	
	std::string ast_construct_head::fetch_name() const
	{
		return "head";
	}
	
	s_ptr<const sl_type> ast_construct_head::fetch_type(const typecontext& c) const
	{
		const s_ptr<const sl_type> a(c.create_fresh(sl));
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			make_s<sl_type_array>(a, sl)
		));
		
		return make_s<sl_type_function>(args, a, sl);
	}
	
	s_ptr<const ir_stmt> ast_construct_head::translate(const ir_label l_construct, const ircontext& c) const
	{
		const s_ptr<const ir_exp> e_return(make_s<ir_exp_mem>(
			make_s<ir_exp_binop>(
				ir_exp_binop::op_plus,
				make_s<ir_exp_temp>(c.frame_reg),
				make_s<ir_exp_const>(1)
			)
		));
	
		s_ptr<const ir_stmt> r(make_s<ir_stmt_label>(l_construct));
		
		ir_stmt::cat(r, ir_stmt::push(create_vector_ptr<const ir_exp>(
			make_s<ir_exp_mem>(
				make_s<ir_exp_mem>(make_s<ir_exp_binop>(
					ir_exp_binop::op_plus,
					make_s<ir_exp_temp>(c.frame_reg),
					make_s<ir_exp_const>(2) //First argument on stack
				))
			)
		), c));
		
		ir_stmt::cat(r, make_s<ir_stmt_jump>(e_return));
		
		return r;
	}
	
	std::string ast_construct_tail::fetch_name() const
	{
		return "tail";
	}
	
	s_ptr<const sl_type> ast_construct_tail::fetch_type(const typecontext& c) const
	{
		const s_ptr<const sl_type> r(make_s<sl_type_array>(c.create_fresh(sl), sl));
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			s_ptr<const sl_type>(r)
		));
		
		return make_s<sl_type_function>(args, r, sl);
	}
	
	s_ptr<const ir_stmt> ast_construct_tail::translate(const ir_label l_construct, const ircontext& c) const
	{
		const s_ptr<const ir_exp> e_return(make_s<ir_exp_mem>(
			make_s<ir_exp_binop>(
				ir_exp_binop::op_plus,
				make_s<ir_exp_temp>(c.frame_reg),
				make_s<ir_exp_const>(1)
			)
		));
	
		s_ptr<const ir_stmt> r(make_s<ir_stmt_label>(l_construct));
		
		ir_stmt::cat(r, ir_stmt::push(create_vector_ptr<const ir_exp>(
			make_s<ir_exp_mem>(
				make_s<ir_exp_binop>(
					ir_exp_binop::op_plus,
					make_s<ir_exp_mem>(make_s<ir_exp_binop>(
						ir_exp_binop::op_plus,
						make_s<ir_exp_temp>(c.frame_reg),
						make_s<ir_exp_const>(2) //First argument on stack
					)),
					make_s<ir_exp_const>(1)
				)
			)
		), c));
		
		ir_stmt::cat(r, make_s<ir_stmt_jump>(e_return));
		
		return r;
	}
	
	std::string ast_construct_fst::fetch_name() const
	{
		return "fst";
	}
	
	s_ptr<const sl_type> ast_construct_fst::fetch_type(const typecontext& c) const
	{
		const s_ptr<const sl_type> a1 = std::static_pointer_cast<const sl_type>(c.create_fresh(sl));
		const s_ptr<const sl_type> a2 = std::static_pointer_cast<const sl_type>(c.create_fresh(sl));
		
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			make_s<sl_type_tuple>(a1, a2, sl)
		));
		
		return make_s<sl_type_function>(args, a1, sl);
	}
	
	s_ptr<const ir_stmt> ast_construct_fst::translate(const ir_label l_construct, const ircontext& c) const
	{
		const s_ptr<const ir_exp> e_return(make_s<ir_exp_mem>(
			make_s<ir_exp_binop>(
				ir_exp_binop::op_plus,
				make_s<ir_exp_temp>(c.frame_reg),
				make_s<ir_exp_const>(1)
			)
		));
	
		s_ptr<const ir_stmt> r(make_s<ir_stmt_label>(l_construct));
		
		ir_stmt::cat(r, ir_stmt::push(create_vector_ptr<const ir_exp>(
			make_s<ir_exp_mem>(
				make_s<ir_exp_mem>(make_s<ir_exp_binop>(
					ir_exp_binop::op_plus,
					make_s<ir_exp_temp>(c.frame_reg),
					make_s<ir_exp_const>(2) //First argument on stack
				))
			)
		), c));
		
		ir_stmt::cat(r, make_s<ir_stmt_jump>(e_return));
		
		return r;
	}
	
	std::string ast_construct_snd::fetch_name() const
	{
		return "snd";
	}
	
	s_ptr<const sl_type> ast_construct_snd::fetch_type(const typecontext& c) const
	{
		const s_ptr<const sl_type> a1 = std::static_pointer_cast<const sl_type>(c.create_fresh(sl));
		const s_ptr<const sl_type> a2 = std::static_pointer_cast<const sl_type>(c.create_fresh(sl));
		
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			make_s<sl_type_tuple>(a1, a2, sl)
		));
		
		return make_s<sl_type_function>(args, a2, sl);
	}
	
	s_ptr<const ir_stmt> ast_construct_snd::translate(const ir_label l_construct, const ircontext& c) const
	{
		const s_ptr<const ir_exp> e_return(make_s<ir_exp_mem>(
			make_s<ir_exp_binop>(
				ir_exp_binop::op_plus,
				make_s<ir_exp_temp>(c.frame_reg),
				make_s<ir_exp_const>(1)
			)
		));
	
		s_ptr<const ir_stmt> r(make_s<ir_stmt_label>(l_construct));
		
		ir_stmt::cat(r, ir_stmt::push(create_vector_ptr<const ir_exp>(
			make_s<ir_exp_mem>(
				make_s<ir_exp_binop>(
					ir_exp_binop::op_plus,
					make_s<ir_exp_mem>(make_s<ir_exp_binop>(
						ir_exp_binop::op_plus,
						make_s<ir_exp_temp>(c.frame_reg),
						make_s<ir_exp_const>(2) //First argument on stack
					)),
					make_s<ir_exp_const>(1)
				)
			)
		), c));
		
		ir_stmt::cat(r, make_s<ir_stmt_jump>(e_return));
		
		return r;
	}
}

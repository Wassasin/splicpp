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
		
		const s_ptr<const sl_type> r(new sl_type_void(sl));
		
		return s_ptr<const sl_type>(new sl_type_function(args, r, sl));
	}
	
	s_ptr<const ir_stmt> ast_construct_print::translate(const ir_label l_construct, const ircontext& c) const
	{
		s_ptr<const ir_stmt> r(ir_stmt_label::create(l_construct));
		
		//TODO - No IR-code available to interact with console
		
		ir_stmt::cat(r, ir_stmt_jump::create(
			ir_exp_mem::create(
				ir_exp_binop::create(
					ir_exp_binop::op_plus,
					ir_exp_temp::create(c.frame_reg),
					ir_exp_const::create(1)
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
			s_ptr<const sl_type>(new sl_type_array(std::static_pointer_cast<const sl_type>(c.create_fresh(sl)), sl))
		));
		
		const s_ptr<const sl_type> r(new sl_type_bool(sl));
		
		return s_ptr<const sl_type>(new sl_type_function(args, r, sl));
	}
	
	s_ptr<const ir_stmt> ast_construct_is_empty::translate(const ir_label l_construct, const ircontext& c) const
	{
		ir_label l_true = c.create_label();
		ir_label l_false = c.create_label();
		
		const s_ptr<const ir_exp> e_return(ir_exp_mem::create(
			ir_exp_binop::create(
				ir_exp_binop::op_plus,
				ir_exp_temp::create(c.frame_reg),
				ir_exp_const::create(1)
			)
		));
	
		s_ptr<const ir_stmt> r(ir_stmt_label::create(l_construct));
		
		ir_stmt::cat(r, ir_stmt_cjump::create(
			ir_stmt_cjump::op_eq,
			ir_exp_mem::create(ir_exp_binop::create(
					ir_exp_binop::op_plus,
					ir_exp_temp::create(c.frame_reg),
					ir_exp_const::create(2) //First argument on stack
			)),
			ir_exp_name::create(c.l_nil),
			l_true,
			l_false
		));
				
		ir_stmt::cat(r, ir_stmt_label::create(l_true));
		ir_stmt::cat(r, ir_stmt::push(create_vector_ptr<const ir_exp>(ir_exp_const::create(true)), c));
		ir_stmt::cat(r, ir_stmt_jump::create(e_return));
		
		ir_stmt::cat(r, ir_stmt_label::create(l_false));
		ir_stmt::cat(r, ir_stmt::push(create_vector_ptr<const ir_exp>(ir_exp_const::create(false)), c));
		ir_stmt::cat(r, ir_stmt_jump::create(e_return));
		
		return r;
	}
	
	std::string ast_construct_head::fetch_name() const
	{
		return "head";
	}
	
	s_ptr<const sl_type> ast_construct_head::fetch_type(const typecontext& c) const
	{
		const s_ptr<const sl_type> a = std::static_pointer_cast<const sl_type>(c.create_fresh(sl));
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			s_ptr<const sl_type>(new sl_type_array(a, sl))
		));
		
		return s_ptr<const sl_type>(new sl_type_function(args, a, sl));
	}
	
	s_ptr<const ir_stmt> ast_construct_head::translate(const ir_label l_construct, const ircontext& c) const
	{
		const s_ptr<const ir_exp> e_return(ir_exp_mem::create(
			ir_exp_binop::create(
				ir_exp_binop::op_plus,
				ir_exp_temp::create(c.frame_reg),
				ir_exp_const::create(1)
			)
		));
	
		s_ptr<const ir_stmt> r(ir_stmt_label::create(l_construct));
		
		ir_stmt::cat(r, ir_stmt::push(create_vector_ptr<const ir_exp>(
			ir_exp_mem::create(
				ir_exp_mem::create(ir_exp_binop::create(
					ir_exp_binop::op_plus,
					ir_exp_temp::create(c.frame_reg),
					ir_exp_const::create(2) //First argument on stack
				))
			)
		), c));
		
		ir_stmt::cat(r, ir_stmt_jump::create(e_return));
		
		return r;
	}
	
	std::string ast_construct_tail::fetch_name() const
	{
		return "tail";
	}
	
	s_ptr<const sl_type> ast_construct_tail::fetch_type(const typecontext& c) const
	{
		const s_ptr<const sl_type> r(new sl_type_array(c.create_fresh(sl), sl));
		const std::vector<s_ptr<const sl_type>> args(create_vector_ptr<const sl_type>(
			s_ptr<const sl_type>(r)
		));
		
		return s_ptr<const sl_type>(new sl_type_function(args, r, sl));
	}
	
	s_ptr<const ir_stmt> ast_construct_tail::translate(const ir_label l_construct, const ircontext& c) const
	{
		const s_ptr<const ir_exp> e_return(ir_exp_mem::create(
			ir_exp_binop::create(
				ir_exp_binop::op_plus,
				ir_exp_temp::create(c.frame_reg),
				ir_exp_const::create(1)
			)
		));
	
		s_ptr<const ir_stmt> r(ir_stmt_label::create(l_construct));
		
		ir_stmt::cat(r, ir_stmt::push(create_vector_ptr<const ir_exp>(
			ir_exp_mem::create(
				ir_exp_binop::create(
					ir_exp_binop::op_plus,
					ir_exp_mem::create(ir_exp_binop::create(
						ir_exp_binop::op_plus,
						ir_exp_temp::create(c.frame_reg),
						ir_exp_const::create(2) //First argument on stack
					)),
					ir_exp_const::create(1)
				)
			)
		), c));
		
		ir_stmt::cat(r, ir_stmt_jump::create(e_return));
		
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
			s_ptr<const sl_type>(new sl_type_tuple(a1, a2, sl))
		));
		
		return s_ptr<const sl_type>(new sl_type_function(args, a1, sl));
	}
	
	s_ptr<const ir_stmt> ast_construct_fst::translate(const ir_label l_construct, const ircontext& c) const
	{
		const s_ptr<const ir_exp> e_return(ir_exp_mem::create(
			ir_exp_binop::create(
				ir_exp_binop::op_plus,
				ir_exp_temp::create(c.frame_reg),
				ir_exp_const::create(1)
			)
		));
	
		s_ptr<const ir_stmt> r(ir_stmt_label::create(l_construct));
		
		ir_stmt::cat(r, ir_stmt::push(create_vector_ptr<const ir_exp>(
			ir_exp_mem::create(
				ir_exp_mem::create(ir_exp_binop::create(
					ir_exp_binop::op_plus,
					ir_exp_temp::create(c.frame_reg),
					ir_exp_const::create(2) //First argument on stack
				))
			)
		), c));
		
		ir_stmt::cat(r, ir_stmt_jump::create(e_return));
		
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
			s_ptr<const sl_type>(new sl_type_tuple(a1, a2, sl))
		));
		
		return s_ptr<const sl_type>(new sl_type_function(args, a2, sl));
	}
	
	s_ptr<const ir_stmt> ast_construct_snd::translate(const ir_label l_construct, const ircontext& c) const
	{
		const s_ptr<const ir_exp> e_return(ir_exp_mem::create(
			ir_exp_binop::create(
				ir_exp_binop::op_plus,
				ir_exp_temp::create(c.frame_reg),
				ir_exp_const::create(1)
			)
		));
	
		s_ptr<const ir_stmt> r(ir_stmt_label::create(l_construct));
		
		ir_stmt::cat(r, ir_stmt::push(create_vector_ptr<const ir_exp>(
			ir_exp_mem::create(
				ir_exp_binop::create(
					ir_exp_binop::op_plus,
					ir_exp_mem::create(ir_exp_binop::create(
						ir_exp_binop::op_plus,
						ir_exp_temp::create(c.frame_reg),
						ir_exp_const::create(2) //First argument on stack
					)),
					ir_exp_const::create(1)
				)
			)
		), c));
		
		ir_stmt::cat(r, ir_stmt_jump::create(e_return));
		
		return r;
	}
}

#include <iostream>
#include <boost/program_options.hpp>

#include "common/io.hpp"
#include "parser/spl_parser.hpp"
#include "ast/ast_prog.hpp"
#include "typing/symboltable.hpp"
#include "typing/unification_error.hpp"
#include "ir/ircontext.hpp"
#include "ir/ir_stmt.hpp"

#include "mappers/ir_desequencer.hpp"
#include "mappers/ir_call_transformer.hpp"

int main(int argc, char **argv)
{
	std::string f = "";
	uint phase = 5;

	boost::program_options::options_description o_general("General options");
	o_general.add_options()
	("help,h", "display this message");

	boost::program_options::options_description o_debug("Debugging options");
	o_debug.add_options()
	("grammar", "prints the generated grammar")
	("ptable", "prints the SPL parser table")
	("clean", "clears the SPL parser table cache");
	
	boost::program_options::options_description o_file("File options");
	o_file.add_options()
	("print", "pretty-prints the parsed file (implies -p1)")
	("phase,p", boost::program_options::value<decltype(phase)>(&phase), "phase until which to parse the file, see below");

	boost::program_options::options_description o_hidden("Hidden options");
	o_hidden.add_options()
	("file", boost::program_options::value<decltype(f)>(&f), "file to be parsed");
	
	boost::program_options::variables_map vm;
	boost::program_options::positional_options_description pos;
	pos.add("file", -1);

	boost::program_options::options_description options("Allowed options");
	options.add(o_general).add(o_debug).add(o_file).add(o_hidden);
	
	try
	{
		boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(options).positional(pos).run(), vm);
	} catch(boost::program_options::unknown_option &e)
	{
		std::cerr << "Unknown option --" << e.get_option_name() << ", see --help" << std::endl;
		return -1;
	}
	
	if(vm.count("help"))
	{
		std::cout << "Simple Programming Language in C++ Compiler by Wouter Geraedts [https://github.com/Wassasin/splicpp]" << std::endl;
		std::cout << "Usage: ./splicpp [options] [file]" << std::endl << std::endl;
		std::cout << o_general << o_debug << o_file << std::endl;
		
		std::cout << "Phases:" << std::endl;
		std::cout << "  1: parsing" << std::endl;
		std::cout << "  2: scope analysis" << std::endl;
		std::cout << "  3: type inferencing" << std::endl;
		std::cout << "  4: IL generation (TODO)" << std::endl;
		std::cout << "  5: SPL bytecode generation (TODO)" << std::endl;
		
		return 0;
	}
	
	try
	{
		boost::program_options::notify(vm);
	} catch(const boost::program_options::required_option &e)
	{
		std::cerr << "You forgot this: " << e.what() << std::endl;
		return -1;
	}
	
	
	splicpp::spl_parser p;
	
	if(vm.count("grammar"))
	{
		p.print_g();
		return 0;
	}
	
	if(vm.count("ptable"))
	{
		p.print_t();
		return 0;
	}
	
	if(f != "")
	{
		if(phase < 1)
			return 0;
		
		std::string str = splicpp::readfile(f);
		auto prog = p.parse(str);
	
		if(vm.count("print"))
			prog->pretty_print(std::cout, 0);

		if(phase < 2)
			return 0;
		
		splicpp::symboltable s;	
		prog->register_ids(s);
		
		if(phase < 3)
			return 0;
		
		try
		{
			s.check_types();
		} catch (splicpp::unification_error& e)
		{
			e.print(str, std::cerr);
			std::cerr << std::endl;
			return -1;
		}
		
		if(phase < 4)
			return 0;
		
		splicpp::ircontext c;
		for(const auto stmt : splicpp::ir_call_transformer::apply(splicpp::ir_desequencer::desequence(prog->translate(c, s)), c))
		{
			stmt->print(std::cout, 0);
			std::cout << std::endl;
		}
		
		return 0;
	}
	
	std::cerr << "Done nothing; see --help" << std::endl;	
	return -1;
}

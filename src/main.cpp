#include <iostream>
#include <boost/program_options.hpp>

#include "common/io.hpp"
#include "parser/spl_parser.hpp"
#include "ast/ast_prog.hpp"
#include "typing/symboltable.hpp"

int main(int argc, char **argv)
{
	std::string f = "../src/tests/parser/fac.spl";

	boost::program_options::options_description options("Options");

	options.add_options()
	("help,h", "display this message")
	("pptable,p", "prints the SPL parser table")
	("pgrammar,g", "prints the generated grammar")
	("file,f", boost::program_options::value<decltype(f)>(&f), "file to be parsed")
	//("clean,c", "forces regeneration of ptable")
	;

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), vm);

	if(vm.count("help"))
	{
		std::cout << "Simple Programming Language parser by Wouter Geraedts [https://github.com/Wassasin/splicpp]" << std::endl;
		std::cout << options;
		return 0;
	}
	
	try
	{
		boost::program_options::notify(vm);
	} catch(boost::program_options::required_option &e)
	{
		std::cout << "you forgot this: " << e.what() << std::endl;
		return -1;
	}
	
	
	splicpp::spl_parser p;
	
	if(vm.count("pgrammar"))
		p.print_g();
	
	if(vm.count("pptable"))
		p.print_t();
	
	auto prog = p.parse(splicpp::readfile(f));
	
	prog->pretty_print(std::cout, 0);
	
	splicpp::symboltable s;
	
	prog->register_ids(s);
	s.check_types();
	
	return 0;
}

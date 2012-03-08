#include <iostream>
#include <boost/program_options.hpp>

#include "common/io.hpp"
#include "parser/spl_parser.hpp"

int main(int argc, char **argv)
{
	std::string f = "../src/tests/parser/fac.spl";

	boost::program_options::options_description options("Options");

	options.add_options()
	("help,h", "display this message")
	("ptable,p", "prints the SPL parser table")
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
	
	if(vm.count("ptable"))
		p.print_t();
	
	//splicpp::grammar tmp;
	
	p.parse(splicpp::readfile(f));
	
	return 0;
}

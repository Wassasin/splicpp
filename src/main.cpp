#include <iostream>
#include <boost/program_options.hpp>

#include "parser/spl_parser.hpp"

int main(int argc, char **argv)
{
	uint phase = 1;

	boost::program_options::options_description options("Options");

	options.add_options()
	("help,h", "display this message")
	("phase,p", boost::program_options::value<decltype(phase)>(&phase), "which phase the compiler proceeds (1=parser, 2=typeinference, 3=codegeneration)")
	;

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, options), vm);

	if(vm.count("help"))
	{
		std::cout << options << std::endl;
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
	
	//splicpp::grammar tmp;
	
	//p.parse(tmp, lang);
	
	return 0;
}

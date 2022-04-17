#include "Compiler.h"	

void ui_handler(int argc, char* argv[])
{
	if (argc < 3)
	{
		throw std::string("\033[31mError: missing inputs: Use following pattern: /TuringLanguage input_file_name\toutput_file_name\n\033[0m");
	}
	
	if (argc > 3)
	{
		throw std::string("\033[31mError: to many inputs: Use following pattern: /TuringLanguage input_file_name\toutput_file_name\n\033[0m");
	}
		
	std::ifstream file;
	file.open(argv[1]);
	
	if (file.fail())
	{
		throw std::string("\033[31mError: invalid input file name or directory\n\033[0m");
	}
	
	std::ofstream output_file_name(argv[2]);

	if (output_file_name.fail())
	{
		throw std::string("\033[31mError: invalid output file name or directory\n\033[0m");
	}
	
	std::stringstream sstream;
	sstream << file.rdbuf();	
	file.close();
		
	Compiler compiler(sstream);

	std::string source_code =  compiler.compile();
	
	output_file_name << source_code;
	output_file_name.close(); 
}

int main(int argc, char* argv[])
{
	try
	{
		ui_handler(argc, argv);		
	}
	catch(const std::string report)
	{
		std::cerr << report;
		return 1;
	}
	
	return 0;
}


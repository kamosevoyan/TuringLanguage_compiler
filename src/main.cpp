#include "Compiler.h"	

void ui_handler(int argc, char* argv[])
{
	if (argc < 3)
	{
		throw std::string("Error: missing inputs: Use following pattern: /TuringLanguage input_file_name\toutput_file_name.");
	}
	
	if (argc > 3)
	{
		throw std::string("Error: to many inputs: Use following pattern: /TuringLanguage input_file_name\toutput_file_name.");
	}
		
	std::ifstream file;
	file.open(argv[1]);
	
	if (file.fail())
	{
		throw std::string("Error: invalid input file name or directory.");
	}
	
	std::ofstream output_file_name(argv[2]);

	if (output_file_name.fail())
	{
		throw std::string("Error: invalid output file name or directory.");
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
	catch(const std::string& report)
	{
		std::cerr << "\033[31m" << report << "\n\033[0m";
		return 1;
	}
	
	return 0;
}


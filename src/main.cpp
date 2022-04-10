#include "TuringLanguage.h"
#define DEBUG


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


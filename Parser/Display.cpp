/////////////////////////////////////////////////////////////////////
//  Display.CPP - Displays results                                 //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, feb2014               //    
//  Author:			Avinash Gudapati, Fall 2013,agudapat@syr.edu   //       
/////////////////////////////////////////////////////////////////////

#include "Display.h"

void Display::displayMatching(std::vector<std::pair<element,element> > v1, std::vector<std::pair<std::string, std::string>> v2){
	std::cout << "Matching lines\t\t" << "File Name\t\t" << "Scope Type and Scope Name\n";
	std::cout << "===============================================================================\n";
	for (int i = 0; i < (int)(v1.size()); i++){
		std::string firstScopeLines = adjustSize(std::to_string(v1.at(i).first.ScopeStartLine) + " to " + std::to_string(v1.at(i).first.ScopeEndLine));
		std::string secondScopeLines = adjustSize(std::to_string(v1.at(i).second.ScopeStartLine) + " to " + std::to_string(v1.at(i).second.ScopeEndLine));
		std::string filename1 = adjustSize(v2.at(i).first);
		std::string filename2 = adjustSize(v2.at(i).second);
		std::cout << firstScopeLines << "\t\t"<< filename1 << "\t\t" << v1.at(i).first.type + " " + v1.at(i).first.name << "\n";
		std::cout << secondScopeLines << "\t\t"<< filename2 << "\t\t" << v1.at(i).second.type + " " + v1.at(i).second.name << "\n\n\n";
	}
}
void Display::displayTimer(long val){
	std::cout << "\n=============================================================\n";
	std::cout << "The Elapsed Time is:" << val << "\n";
	std::cout << "=============================================================\n";
}
std::string Display::adjustSize(std::string inputString){
	while (inputString.length() < 15){
		inputString.append(" ");
	}
	return inputString;
}
#ifdef TEST_DISPLAY

#include <iostream>

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing DISPLAY class\n "
		<< std::string(10, '=') << std::endl;
	std::vector<std::pair<element,element> > v1 = __argv[1];
	std::vector<std::pair<std::string, std::string>> v2 = __argv[2];
	long val = 1000000;
	Display *dis = new Display();
	dis->displayMatching(v1,v2);
	dis->displayTimer(val);
}
#endif
/////////////////////////////////////////////////////////////////////
//  TestExecutive.cpp - Tests All the functionalities           //
//  ver 1.4                                                        //
//   Language:      Visual C++ 2013                                //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, feb2014               //    
//  Author:			Avinash Gudapati, Fall 2013,agudapat@syr.edu   //
//  Reference:      Jim Fawcett, CST 4-187, Syracuse University    //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "FoldingRules.h"
#include "ConsoleProcessing.h"
#include "FileSystem.h"
#include "ChronoTimer.h"
#include "StructElement.h"


//----< test stub >--------------------------------------------


#include <queue>
using  ConProc = ConsoleProcessing;
int main(int argc, char* argv[])
{
	try
	{
		//printing command line args and retrieving files on which scope analysis is to be performed
		std::cout << ConProc::Title("Demonstrate Console Processing") << std::endl;
		ConProc::showCommandLine(argc, argv);
		ConProc::showCommandLineParse(argc, argv);
		ConProc::CommandLine cl = ConProc::parseCommandLine(argc, argv);
		ConsoleProcessing::Files inputFiles = ConProc::showFiles(std::get<0>(cl), std::get<1>(cl), std::get<2>(cl));
		ConProc::Options option = std::get<2>(cl);
		std::cout << "\n";
		//if nofiles are present
		if(inputFiles.size() == 0)
		{
			std::cout << "\n  There are no files in the given path matching the pattern \n\n";
			return 1;
		}
		else{
			for (auto file : inputFiles){
				std::cout <<"\t"<< FileSystem::Path::getName(file) << "\n";
			}
		}
		storeScopes ss;
	
		//start for processing files loop
		for(int i=0; i<(int)inputFiles.size(); ++i)
		{
			ConfigParseToConsole configure;
			Parser* pParser = configure.Build();
    
		   if(pParser)
			{
				if (!configure.Attach(inputFiles[i]))
				{
					std::cout << "\n  could not open file " << inputFiles[i] << std::endl;
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return 1;
			}
			// now that parser is built, use it
			while(pParser->next())
				pParser->parse();

			//call to build xml representation of scopes
			
			ss.mainVector.push_back(configure.FileScopeBuilder(FileSystem::Path::getName(inputFiles[i])));
			std::cout << "\n\n";

			if (i == (inputFiles.size() - 1)){
				FileComparision *fc= new FileComparision();
				fc->compareFiles(ss.mainVector);
			}
	  }
	}
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  
}


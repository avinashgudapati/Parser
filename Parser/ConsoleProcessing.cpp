/////////////////////////////////////////////////////////////////////
// ConsoleProcessing.cpp - Support for Console Applications        //
// ver 1.1                                                         //
// Language:      Visual C++ 2013                                  //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, feb2014               //    
//  Author:			Avinash Gudapati, Fall 2013,agudapat@syr.edu   //
//  Reference:      Jim Fawcett, CST 4-187, Syracuse University    //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include "ConsoleProcessing.h"
#include "FileSystem.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <tuple>
#include <stdlib.h>  /* show environment */

//----< return title string with underlines >------------------------------

std::string ConsoleProcessing::Title(const std::string& title, char underline)
{
  std::string rtn = "\n  " + title + "\n ";
  std::string temp = std::string(title.size() + 2, underline);
  return rtn += temp;
}
//----< display environment variables on console >-------------------------

void ConsoleProcessing::showEnvironment()
{
  std::cout << Title("Environment Variables:",'-');
  extern char** environ;  // defined in stdlib.h
 
  for (char** current = environ; *current; current++)
    std::cout << "\n  " << *current;
}
//----< is this execution running from Visual Studio? >--------------------

bool ConsoleProcessing::IsRunningInVisualStudio()
{
  extern char** environ;  // defined in stdlib.h
  for (char** current = environ; *current; current++)
  {
    if (strstr(*current, "VisualStudioDir") != nullptr)
      return true;
  }
  return false;
}
//----< display command line aguments on console >-------------------------

void ConsoleProcessing::showCommandLine(int argc, char* argv[])
{
  std::cout << Title("Command line arguments:", '-') << "\n  ";
  std::vector<char*> args(&argv[1], &argv[0] + argc);
  for (auto arg : args)
    std::cout << "\"" << arg << "\" ";
  std::cout << "\n";
}
//----< return console's current path >------------------------------------

ConsoleProcessing::Path ConsoleProcessing::currentPath()
{
  return FileSystem::Directory::getCurrentDirectory();
}
//----< display files on path matching patts >-----------------------------

ConsoleProcessing::Files ConsoleProcessing::showFiles(const Path& path, Patterns& patts, Options &option)
{
	Files files;
  if (!FileSystem::Directory::exists(path))
  {
    std::ostringstream out;
    out << "path " << path << "does not exist";
    throw std::exception(out.str().c_str());
  }
  if (patts.size() == 0)
    patts.push_back("*.*");
	std::ostringstream out;
	out << "Files on " << path << " are:";
	std::cout << "\n  " << Title(out.str(), '=');
	std::vector<std::string> directories;
	directories.push_back(std::string(path));
	for (auto opt : option){
		if (opt == "/S" || opt == "/s"){                                    //getting all directories
			showAllDirectories(std::string(path), directories);
		}																			
	}
	for (auto direc : directories){
		FileSystem::Directory::setCurrentDirectory(direc);
		for (auto patt : patts)
		{
			std::string fp = FileSystem::Directory::getCurrentDirectory();
			std::vector<std::string> temp = FileSystem::Directory::getFiles(FileSystem::Directory::getCurrentDirectory(), patt);
			for (int i = 0; i < (int)(temp.size());i++){
				temp[i] = fp +"\\"+ temp[i];                                    //retrieving files
			}
			files.insert(files.end(), temp.begin(), temp.end());
		}
	}
	return files;
}

std::vector<std::string> ConsoleProcessing::showAllDirectories(std::string & baseDirectory, std::vector<std::string> & directories){
	std::vector<std::string> temp_directories = FileSystem::Directory::getDirectories(baseDirectory);
	std::vector<std::string> temp1_directories;
	if (temp_directories.size() < 2){
		return directories;
	}
	else{
		for (int i = 2; i < (int)(temp_directories.size()); i++){
			directories.push_back(baseDirectory + '\\' + temp_directories[i]);
			temp1_directories.push_back(baseDirectory + '\\' + temp_directories[i]);
		}
		int count = 0;                                                                //recursive method to retrieve all directories
		while (count < (int)(temp1_directories.size())){
			showAllDirectories(temp1_directories[count], directories);
			count++;
		}
	}
	return directories;
	
}
//----< helper function - is this token an option? >-----------------------

bool ConsoleProcessing::IsOption(const std::string& token)
{
  return (token[0] == '/');
}
//----< return tuple { path, patterns, options } from commandline >--------

ConsoleProcessing::CommandLine ConsoleProcessing::parseCommandLine(int argc, char* argv[])
{
  Options options;
  Patterns patterns;
  Path path;
  std::vector<std::string> vc;
  for (int i = 1; i < argc; i++){
	  
	  vc.push_back(argv[i]);
  }
  if (argc == 1)
    path = FileSystem::Path::getFullFileSpec(".");  // default path

  for (int i = 1; i < argc; ++i)
  {
    if (IsOption(argv[i]))
      options.push_back(argv[i]);
    else
    {
      if (path.size() == 0)
        path = FileSystem::Path::getFullFileSpec(argv[i]);
      else
        patterns.push_back(argv[i]);
    }
  }
  return CommandLine { path, patterns, options };
}
//----< display { path, patterns, options } on console >-------------------

void ConsoleProcessing::showCommandLineParse(int argc, char* argv[])
{
  std::cout << Title("\nParsing Command Line:", '=');

  CommandLine cl = parseCommandLine(argc, argv);

  // display path, patterns, and options
  std::cout << "\n  Path:\t " << std::get<0>(cl);
  std::cout << "\n  Patterns:\t ";
  for (auto pattern : std::get<1>(cl))
    std::cout << pattern << " ";
  std::cout << "\n  Options:\t ";
  for (auto option : std::get<2>(cl))
    std::cout << option << " ";
  std::cout << "\n";
}

//----< test stub >--------------------------------------------------------

#ifdef TEST_CONSOLEPROCESSING

using  ConProc = ConsoleProcessing;

int main(int argc, char* argv[])
{
  std::cout << ConProc::Title("Demonstrate Console Processing") << std::endl;
  ConProc::showCommandLine(argc, argv);
  std::cout << ConProc::Title("current path:", '-') << "\n  " + ConProc::currentPath() + "\n";
  ConProc::showEnvironment();
  if (ConProc::IsRunningInVisualStudio())
    std::cout << "\n\n  -- Running in Visual Studio --\n";
  else
    std::cout << "\n\n  -- Not running in Visual Studio --\n";

  ConProc::showCommandLineParse(argc, argv);
  ConProc::CommandLine cl = ConProc::parseCommandLine(argc, argv);
  ConProc::showFiles(std::get<0>(cl), std::get<1>(cl), std::get<2>(cl));
  std::cout << "\n\n";
}

#endif

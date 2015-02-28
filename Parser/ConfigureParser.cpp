/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers            //
//  ver 3.0                                                        //
//                                                                 //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, feb2014               //    
//  Author:			Avinash Gudapati, Fall 2013,agudapat@syr.edu   //
//  Reference:      Jim Fawcett, CST 4-187, Syracuse University    //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated

  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pFR;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name,bool isFile)
{
  if(pToker == 0)
    return false;
  return pToker->attach(name, isFile);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::Build()
{
 try
	{	
		pToker = new Toker;
		pToker->returnComments();
		pSemi = new SemiExp(pToker);
		pParser = new Parser(pSemi);
		pRepo = new Repository(pToker);
		pFR = new codeFoldingRules;
		pParser->addFoldingRules(pFR);
		pBeginningOfScope = new BeginningOfScope();
		pHandlePush = new HandlePush(pRepo);
		pBeginningOfScope->addAction(pHandlePush);
		pParser->addRule(pBeginningOfScope);
		pEndOfScope = new EndOfScope();
		pHandlePop = new HandlePop(pRepo);
		pEndOfScope->addAction(pHandlePop);
		pParser->addRule(pEndOfScope);
		pFunctionDefinition = new FunctionDefinition(pRepo); //sent pRepo as argument for newly added paramaterized constructor
		pPushFunction = new PushFunction(pRepo);  // no action
		pFunctionDefinition->addAction(pPushFunction);
		pParser->addRule(pFunctionDefinition);
		pClassDefinition = new ClassDefinition();
		pPushClass = new PushClass(pRepo);
		pClassDefinition->addAction(pPushClass);
		pParser->addRule(pClassDefinition);
		pStructDefinition = new StructDefinition();
		pPushStruct = new PushStruct(pRepo);
		pStructDefinition->addAction(pPushStruct);
		pParser->addRule(pStructDefinition);
		pEnumDefinition = new EnumDefinition();
		pPushEnum = new PushEnum(pRepo);
		pEnumDefinition->addAction(pPushEnum);
		pParser->addRule(pEnumDefinition);
		pNamespaceDefinition = new NamespaceDefinition();
		pPushNamespace = new PushNamespace(pRepo);
		pNamespaceDefinition->addAction(pPushNamespace);
		pParser->addRule(pNamespaceDefinition);
		pFileComparision = new FileComparision();
		return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}
//function to call xml build
std::vector<element> ConfigParseToConsole::FileScopeBuilder(std::string filename)
{
	std::vector<element> mV = pHandlePop->buildFileScopes(filename, pRepo->getFileScopesVector());
	return mV;
}



#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
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
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif

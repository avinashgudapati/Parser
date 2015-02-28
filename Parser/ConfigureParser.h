#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
/////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers              //
//  ver 3.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, feb2014               //    
//  Author:			Avinash Gudapati, Fall 2013,agudapat@syr.edu   //
//  Reference:      Jim Fawcett, CST 4-187, Syracuse University    //
//                 (315) 443-3948, jfawcett@twcny.rr.com          //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module builds and configures parsers.  It builds the parser
  parts and configures them with application specific rules and actions.

  Public Interface:
  =================
  ConfigParseToConsole conConfig;
  conConfig.Build();
  conConfig.Attach(someFileName);
  conConfig.XmlBuild();
  conConfig.treeBuild();

  Build Process:
  ==============
  Required files
    - ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp,
      SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv Project1HelpS06.sln
    - cl /EHsc /DTEST_PARSER ConfigureParser.cpp parser.cpp \
         ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 3.0 : 16 Feb 14
  - Added methods for building XML and TREE structures for scopes in function
  ver 2.0 : 01 Jun 11
  - Major revisions to begin building a strong code analyzer
  ver 1.1 : 01 Feb 06
  - cosmetic changes to ConfigureParser.cpp
  ver 1.0 : 12 Jan 06
  - first release

*/

//
#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "FoldingRules.h"
#include "FileComparision.h"

///////////////////////////////////////////////////////////////
// build parser that writes its output to console

class ConfigParseToConsole : IBuilder
{
public:
  ConfigParseToConsole() {};
  ~ConfigParseToConsole();
  bool Attach(const std::string& name,bool isFile = true);
  Parser* Build();
  std::vector<element> FileScopeBuilder(std::string filename);

private:
  // Builder must hold onto all the pieces

  Toker* pToker;
  SemiExp* pSemi;
  Parser* pParser;
  Repository* pRepo;

  // add folding rules

  FoldingRules* pFR;

  // add Rules and Actions

  BeginningOfScope* pBeginningOfScope;
  HandlePush* pHandlePush;
  EndOfScope* pEndOfScope;
  HandlePop* pHandlePop;
  FunctionDefinition* pFunctionDefinition;
  PushFunction* pPushFunction;


  ClassDefinition *pClassDefinition;
  PushClass* pPushClass;

  StructDefinition *pStructDefinition;
  PushStruct* pPushStruct;

  EnumDefinition *pEnumDefinition;
  PushEnum* pPushEnum;

  NamespaceDefinition *pNamespaceDefinition;
  PushNamespace* pPushNamespace;

  FileComparision* pFileComparision;
  // prohibit copies and assignments



  ConfigParseToConsole(const ConfigParseToConsole&);
  ConfigParseToConsole& operator=(const ConfigParseToConsole&);
};


#endif

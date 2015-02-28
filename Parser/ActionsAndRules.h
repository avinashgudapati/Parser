#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 3.0                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, feb2014               //    
//  Author:			Avinash Gudapati, Fall 2013,agudapat@syr.edu   //
//  Reference:      Jim Fawcett, CST 4-187, Syracuse University    //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations:
  ==================
  This module defines several action classes.  Its classes provide
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable.

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
  parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
  - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
  ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
  ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
  - devenv CodeAnalysis.sln
  - cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
  semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 3.0 : 16 Feb 14
  - Added Handle push and pop functionalities for functions,classes, struct, enum, namespace
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
  of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

  */
//
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "ITokCollection.h"
#include "ScopeStack.h"
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "Display.h"
#include "ChronoTimer.h"
#include "StructElement.h"



struct storeScopes
{
	std::vector<std::vector<element>> mainVector;
};
///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.

class Repository  // application specific
{
	ScopeStack<element> stack;
	Toker* p_Toker;
public:
	Repository(Toker* pToker)
	{
		p_Toker = pToker;
	}
	ScopeStack<element>& scopeStack()
	{
		return stack;
	}
	Toker* Toker()
	{
		return p_Toker;
	}
	size_t lineCount()
	{
		return (size_t)(p_Toker->lines());
	}
	
	void pushFileScopesVector(element value){
		FileScopesVector.push_back(value);
	}
	std::vector<element>& getFileScopesVector(){
		return FileScopesVector;
	}

	void pushmainVector(std::vector<element> value){
		mainVector.push_back(value);
	}
	std::vector<std::vector<element>> getmainVector(){
		return mainVector;
	}
private:
	std::vector<element> FileScopesVector;
	std::vector<std::vector<element>> mainVector;
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		//std::cout << "\n--BeginningOfScope rule";
		if (pTc->find("{") < pTc->length())
		{
			doActions(pTc);
			return false;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
	Repository* p_Repos;
public:
	HandlePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		element elem;
		elem.type = "unknown";
		elem.name = "anonymous";
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		//std::cout << "\n--EndOfScope rule";
		if (pTc->find("}") < pTc->length())
		{
			doActions(pTc);
			return false;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
	Repository* p_Repos;
public:
	HandlePop(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		if (p_Repos->scopeStack().size() == 0)
			return;
		element elem = p_Repos->scopeStack().pop();
		if (elem.type == "function")
		{
			doPopActivities(elem);
		}
		if (elem.type == "class")
		{
			doPopActivities(elem);
		}
		if (elem.type == "struct")
		{
			doPopActivities(elem);
		}
		if (elem.type == "enum")
		{
			doPopActivities(elem);
		}
		if (elem.type == "namespace")
		{
			doPopActivities(elem);
		}
		if (elem.type == "SpecialKeyWord"){
			doPopActivities(elem);
		}
	}

	//Performs handling pop activities for all scopes
	void doPopActivities(element e){
		(p_Repos->getFileScopesVector()[e.scopeNumber]).ScopeEndLine = p_Repos->lineCount() + 1;
		(p_Repos->getFileScopesVector()[e.scopeNumber]).ScopeComplexity = (p_Repos->getFileScopesVector().size() - (e.scopeNumber + 1)) / 2;

		element elem1;
		elem1.type = "/" + e.type;
		elem1.name = e.name;
		p_Repos->pushFileScopesVector(elem1);
	}

	std::vector<element> buildFileScopes(std::string filename, std::vector<element> filescopes){
		element elem;
		elem.type = "Filename";
		elem.name = filename;
		filescopes.insert(filescopes.begin(), elem);
		element elem1;
		elem1.type = "/Filename";
		elem1.name = filename;
		filescopes.push_back(elem1);
		p_Repos->pushmainVector(filescopes);
		return filescopes;
	}
};


///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		//std::cout << "\n--PreprocStatement rule";
		if (pTc->find("#") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
	Repository* p_Repos;
public:
	FunctionDefinition(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch", "try", "else", "do" };
		for (int i = 0; i < 8; ++i)
		if (tok == keys[i])
			return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		element item;
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
			{
				//std::cout << "\n--FunctionDefinition rule";
				doActions(pTc);

				return true;
			}
			if ((isSpecialKeyWord(tc[len - 1]) || isSpecialKeyWord(tc[0]))){
				if (isSpecialKeyWord(tc[len - 1])) item.name = tc[len - 1];
				else item.name = tc[0];
				element elem;
				p_Repos->scopeStack().pop();
				elem.type = "SpecialKeyWord";
				elem.name = item.name;
				elem.ScopeStartLine = p_Repos->lineCount();
				elem.scopeNumber = p_Repos->getFileScopesVector().size();
				p_Repos->scopeStack().push(elem);
				//item.name = "b" + item.name;
				p_Repos->pushFileScopesVector(elem);
				
				//p_Repos->setComplexity((p_Repos->getComplexity())+1);
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
	Repository* p_Repos;
public:
	PushFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
			p_Repos->scopeStack().pop();

		// push function scope
		std::string name = (*pTc)[pTc->find("(") - 1];
		element elem;
		elem.type = "function";
		elem.ScopeStartLine = p_Repos->lineCount();
		elem.name = name;
		elem.scopeNumber = p_Repos->getFileScopesVector().size();
		p_Repos->scopeStack().push(elem);
		p_Repos->pushFileScopesVector(elem);
	}
};


///////////////////////////////////////////////////////////////
// rule to detect class definitions

class ClassDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("class") < pTc->length())
		{
			if (pTc->find("{") < pTc->length()){
				doActions(pTc);
				return false;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push class name onto ScopeStack

class PushClass : public IAction
{
	Repository* p_Repos;
public:
	PushClass(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope

		p_Repos->scopeStack().pop();

		// push function scope
		std::string name = (*pTc)[pTc->find("class") + 1];
		element elem;
		elem.type = "class";
		elem.name = name;
		elem.ScopeStartLine = p_Repos->lineCount();
		elem.scopeNumber = p_Repos->getFileScopesVector().size();
		p_Repos->scopeStack().push(elem);
		p_Repos->pushFileScopesVector(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect struct definitions

class StructDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("struct") < pTc->length())
		{
			//std::cout << "\n--FunctionDefinition rule";
			if (pTc->find("{") < pTc->length()){
				doActions(pTc);
				return false;
			}
		}

		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push class name onto ScopeStack

class PushStruct : public IAction
{
	Repository* p_Repos;
public:
	PushStruct(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->scopeStack().pop();

		// push function scope
		std::string name = (*pTc)[pTc->find("struct") + 1];
		element elem;
		elem.type = "struct";
		elem.name = name;
		elem.ScopeStartLine = p_Repos->lineCount();
		elem.scopeNumber = p_Repos->getFileScopesVector().size();
		p_Repos->scopeStack().push(elem);
		p_Repos->pushFileScopesVector(elem);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect enum definitions

class EnumDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("enum") < pTc->length())
		{
			//std::cout << "\n--FunctionDefinition rule";
			if (pTc->find("{") < pTc->length()){
				doActions(pTc);
				return false;
			}
		}

		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push class name onto ScopeStack

class PushEnum : public IAction
{
	Repository* p_Repos;
public:
	PushEnum(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		p_Repos->scopeStack().pop();

		// push function scope
		std::string name = (*pTc)[pTc->find("enum") + 1];
		element elem;
		elem.type = "enum";
		elem.name = name;
		elem.ScopeStartLine = p_Repos->lineCount();
		elem.scopeNumber = p_Repos->getFileScopesVector().size();
		p_Repos->scopeStack().push(elem);
		p_Repos->pushFileScopesVector(elem);
	}
};


///////////////////////////////////////////////////////////////
// rule to detect enum definitions

class NamespaceDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("namespace") < pTc->length())
		{
			//std::cout << "\n--NamespaceDefinition rule";
			if (pTc->find("using") < pTc->length())
			{
				return false;
			}
			doActions(pTc);
			return true;
		}

		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push class name onto ScopeStack

class PushNamespace : public IAction
{
	Repository* p_Repos;
public:
	PushNamespace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		p_Repos->scopeStack().pop();

		// push function scope
		std::string name = (*pTc)[pTc->find("namespace") + 1];
		element elem;
		elem.type = "namespace";
		elem.name = name;
		elem.ScopeStartLine = p_Repos->lineCount();
		elem.scopeNumber = p_Repos->getFileScopesVector().size();
		p_Repos->scopeStack().push(elem);
		p_Repos->pushFileScopesVector(elem);
	}
};


///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n\n  FuncDef Stmt: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		pTc->remove("public");
		pTc->remove(":");
		pTc->trimFront();
		int len = pTc->find(")");
		std::cout << "\n\n  Pretty Stmt:    ";
		for (int i = 0; i < len + 1; ++i)
			std::cout << (*pTc)[i] << " ";
	}
};

#endif

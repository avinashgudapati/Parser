/////////////////////////////////////////////////////////////////////
//  Parser.cpp - Analyzes C++ language constructs		          //
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

//----< register parsing rule >--------------------------------

void Parser::addRule(IRule* pRule)
{
	rules.push_back(pRule);
}
//----< get next ITokCollection >------------------------------

bool Parser::next()
{
	// default operation of doFold(pTokColl) is to simply return
	// - code analysis provides specific rules for handling
	//   for statements and operator expressions which are
	//   bound in ConfigureParser.cpp
	bool succeeded = pTokColl->get();
	if (!succeeded)
		return false;
	pFoldingRules->doFold(pTokColl);
	return true;
}

//----< parse the SemiExp by applying all rules to it >--------

bool Parser::parse()
{
	bool succeeded = false;
	for (size_t i = 0; i<rules.size(); ++i)
	{
		if (rules[i]->doTest(pTokColl))
			succeeded = true;
	}
	return succeeded;
}
//----< register action with a rule >--------------------------

void IRule::addAction(IAction *pAction)
{
	actions.push_back(pAction);
}
//----< invoke all actions associated with a rule >------------

void IRule::doActions(ITokCollection*& pTokColl)
{

	if (actions.size() > 0)
	for (size_t i = 0; i<actions.size(); ++i)
		actions[i]->doAction(pTokColl);
}

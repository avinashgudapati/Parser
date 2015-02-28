#ifndef STRUCTELEMENT_H
#define STRUCTELEMENT_H
/////////////////////////////////////////////////////////////////////
//  ITokCollection.h - package for the ITokCollection interface    //
//  ver 1.2                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, feb2014               //    
//  Author:			Avinash Gudapati, Fall 2013,agudapat@syr.edu   //
//  Reference:      Jim Fawcett, CST 4-187, Syracuse University    //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
Module Purpose :
== == == == == == == =
StructElement is an interface that supports storing all scope type,name,startline,endline and scopecomplexity

Maintenance History :
== == == == == == == == == ==
ver 1.0 : 25 MAR 14
- first release
*/
#include <queue>
#include <string>
#include <sstream>
struct element
{
std::string type;
std::string name;
size_t scopeNumber;
size_t ScopeStartLine;
size_t ScopeEndLine;
size_t ScopeComplexity=0;
std::string show()
{
std::ostringstream temp;
temp << "(";
temp << type;
temp << ", ";
temp << name;
temp << ")";
return temp.str();
}
};
#endif;
/////////////////////////////////////////////////////////////////////
//  FileComparision.h - package for comparing files                //
//  ver 1.2                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, feb2014               //    
//  Author:			Avinash Gudapati, Fall 2013,agudapat@syr.edu   //
/////////////////////////////////////////////////////////////////////
/*
Module Purpose :
== == == == == == == =
Gets input vector which has scopes of all files
Sorts the vector
Compare scopes of each file
Passes matching scopes to display package

Maintenance History :
== == == == == == == == == ==
ver 1.0 : 25 MAR 14
- first release
*/
#ifndef FILECOMPARISION_H
#define FILECOMPARISION_H
#include <queue>
#include <string>
#include <sstream>
#include "StructElement.h"
#include "ActionsandRules.h"
class FileComparision{
	Repository* p_Repos;
public:
	FileComparision(){
	}
	~FileComparision(){	}
	void compareFiles(std::vector<std::vector<element> > inputMainVector);
	void comparing(std::vector<element> f1, std::vector<element> f2, int similaritySize,
	std::vector<std::pair<element, element> > &v1, std::vector<std::pair<std::string, std::string>> &v2);
	std::vector<std::vector<element> > mergesort(std::vector<std::vector<element> >& mainvector);
	std::vector<std::vector<element> > merge(std::vector<std::vector<element> >& leftchild, std::vector<std::vector<element> >& rightchild);
};
#endif;
/////////////////////////////////////////////////////////////////////
//  Display.H - Displays results                                   //
//  ver 1.0                                                        //
//                                                                 //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, feb2014               //    
//  Author:			Avinash Gudapati, Fall 2013,agudapat@syr.edu   //       
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
Performs Output Display

Public Interface:
=================
displayMatching()
	Displays matching files line numbers,scope name, file names
displayFdisplayTimer()
	Displays Time taken for execution

Maintenance History:
====================
ver 1.0 : 25 Mar 14
- first release
*/

#ifndef DISPLAY_H
#define DISPLAY_H
#include <queue>
#include <string>
#include <iostream>
#include "StructElement.h"
class Display {
public:
	Display(){}
	~Display(){}
	void displayMatching(std::vector<std::pair<element,element> > v1, std::vector<std::pair<std::string, std::string> > v2);
	void displayTimer(long val);
	std::string adjustSize(std::string s);
	};
#endif;
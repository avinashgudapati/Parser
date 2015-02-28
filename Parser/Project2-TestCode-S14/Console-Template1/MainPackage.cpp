///////////////////////////////////////////////////////////////////////
// TestCodePr1-S14.cpp - Test scope analyzers                        //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014         //
///////////////////////////////////////////////////////////////////////

#include "Pkg.h"

using namespace Test1;

void Test2::Bracefull::non_inlineTest()
{
  size_t count = 2;
  for (size_t i = 0; i < count; ++i)
  {
    if (true)
    {
      std::cout << "\n  inlineTest function";
      if (false)
      {
        std::cout << "\n  won't get here";
      }
    }
    else {
      std::cout << "\n  won't get here";
    }
    std::cout << "\n  ";
    std::vector<std::string> stuff{ "first", "second", "third" };
    for (auto item : stuff)
    {
      std::cout << item << " ";
    }
  }
}

void Test3::Braceless::non_inlineTest() {
  size_t count = 3;
  for (size_t i = 0; i < count; ++i)
  if (true)
    std::cout << "\n  inlineTest function";
  if (false)
    std::cout << "\n  won't get here";
  else
    std::cout << "\n  will get here";

  std::cout << "\n  ";
  std::vector<std::string> stuff{ "first", "second", "third" };
  for (auto item : stuff)
    std::cout << item << " ";
}


int main(int argc, char* argv[])
{
  std::cout << "\n  Testing Project #1 Scope Analyzers";
  std::cout << "\n ====================================";

  theEasyTest tet;
  tet.someScopes();

  Test2::Bracefull bracefull;
  bracefull.inlineTest();
  bracefull.non_inlineTest();

  Test2::Bracefull::nestedClass nc;
  nc.say();

  Test3::Braceless braceless;
  braceless.inlineTest();
  braceless.non_inlineTest();
  std::cout << "\n\n";
  return 0;
}
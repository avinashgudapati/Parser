/////////////////////////////////////////////////////////////////////
//  FileComparision.cpp - package for comparing files              //
//  ver 1.2                                                        //
//  Language:      Visual C++ 2013                                 //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, feb2014               //    
//  Author:			Avinash Gudapati, Fall 2013,agudapat@syr.edu   //
/////////////////////////////////////////////////////////////////////


#include "FileComparision.h"
void FileComparision::compareFiles(std::vector<std::vector<element> > inputMainVector){
	std::vector<std::vector<element> > mainVector1 = inputMainVector;
	inputMainVector.clear();
	inputMainVector = mergesort(mainVector1);
	int similaritySize =3;
	Display *dis = new Display();
	Timer t;
	t.start();
	std::vector<std::pair<element, element> > v1;
	std::vector<std::pair<std::string, std::string>> v2;
	for (int i = 0; i < (int)(inputMainVector.size()); i++){
		for (int j = (i + 1); j < (int)(inputMainVector.size()); j++){
			comparing(inputMainVector[i], inputMainVector[j], similaritySize, v1, v2);
		}
	}
	t.stop();
	dis->displayMatching(v1, v2);
	dis->displayTimer(t.elapsedTime<std::chrono::nanoseconds>());
}

void FileComparision::comparing(std::vector<element> f1, std::vector<element> f2, int similaritySize,
	std::vector<std::pair<element, element> > &v1, std::vector<std::pair<std::string, std::string>> &v2){
	std::pair<element, element> p1;
	std::pair<std::string, std::string> p2;
	for (int i = 1; i < (int)(f1.size()); i++){
		for (int j = 1; j < (int)(f2.size()); j++){
			if (((f1.at(i).type == f2.at(j).type && f1.at(i).type != "specialkeyword") ||
				(f1.at(i).type == f2.at(j).type && f1.at(i).type == "specialkeyword" && f1.at(i).name == f2.at(j).name))
				&& (int)(f1.at(i).ScopeComplexity) > similaritySize - 1 && f1.at(i).ScopeComplexity == f2.at(j).ScopeComplexity){
				for (int k = 1; k < (int)(f1.at(i)).ScopeComplexity; k++){
					if (f1.at(i + k).type != f2.at(j + k).type){
						if (k>similaritySize - 1){
							p1.first = f1.at(i); p1.second = f2.at(j);
							p2.first = f1.at(0).name; p2.second = f2.at(0).name;
							v1.push_back(p1);
							v2.push_back(p2);
						}
						break;
					}
					if (f1.at(i + k).type == "specialkeyword"){
						if (f1.at(i + k).name != f2.at(j + k).name){
							if (k>similaritySize - 1){
								p1.first = f1.at(i); p1.second = f2.at(j);
								p2.first = f1.at(0).name; p2.second = f2.at(0).name;
								v1.push_back(p1);
								v2.push_back(p2);
							}
							break;
						}
					}
					if (k == f1.at(i).ScopeComplexity - 1){
						p1.first = f1.at(i); p1.second = f2.at(j);
						p2.first = f1.at(0).name; p2.second = f2.at(0).name;
						v1.push_back(p1);
						v2.push_back(p2);
						i = i + k - 1;
					}
				}
			}
		}
	}
}
std::vector<std::vector<element> > FileComparision::mergesort(std::vector<std::vector<element> >& mainvector)
{
	if (mainvector.size() <= 1)
	{
		return mainvector;
	}
	size_t middle = mainvector.size() / 2;
	std::vector<std::vector<element> > leftchild(mainvector.begin(), mainvector.begin() + middle);
	std::vector<std::vector<element> > rightchild(mainvector.begin() + middle, mainvector.begin() + mainvector.size());
	leftchild = mergesort(leftchild);
	rightchild = mergesort(rightchild);
	return merge(leftchild, rightchild);
}
std::vector<std::vector<element> > FileComparision::merge(std::vector<std::vector<element> >& leftchild, std::vector<std::vector<element> >& rightchild)
{
	std::vector<std::vector<element> > copyvector;
	size_t i = 0;
	size_t j = 0;

	while (i < leftchild.size() && j < rightchild.size()){
		if ((leftchild[i].size()) < (rightchild[j].size())){
			copyvector.push_back(leftchild[i]);
			i++;
		}
		else{
			copyvector.push_back(rightchild[j]);
			j++;
		}
	}
	while (i < leftchild.size()){
		copyvector.push_back(leftchild[i]);
		i++;
	}
	while (j < rightchild.size()){
		copyvector.push_back(rightchild[j]);
		j++;
	}
	return copyvector;
}
#ifdef TEST_FILECOMPARISION
#include <queue>
#include <string>
#include <StructElement.h>

int main(int argc, char* argv[])
{
	std::cout << "\n  Demonstrating FileComparision";
	std::cout << "\n ===========================";
	std::vector<std::vector<element> inputMainVector;
	inputMainVector = __argv;
	FileComparision *fc= new FileComparision();
	fc.compareFiles(std::vector<std::vector<element> > inputMainVector);
}
#endif
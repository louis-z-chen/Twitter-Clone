//defines functions defined in setops.h
//same functions as hw2

#include "setops.h"
#include <set>
#include <string>
#include "tweet.h"
using namespace std;


// Computes the intersection of s1 and s2
std::set<Tweet*> operator&(const std::set<Tweet*>& s1, 
                           const std::set<Tweet*>& s2){
	set<Tweet*> temp;
	if(s1.empty() || s2.empty()){
		return temp;
	}
	set<Tweet*> :: iterator firstset;
	//set<Tweet*> :: iterator secondset;
	for(firstset = s1.begin(); firstset != s1.end(); ++firstset){
		//for (secondset = s2.begin(); secondset != s2.end(); ++secondset){
			if(s2.find(*firstset) != s2.end()){
				temp.insert(*firstset);
			}
		//}
	}
	return temp;
}


// Computes the union of s1 and s2
std::set<Tweet*> operator|(const std::set<Tweet*>& s1, 
                           const std::set<Tweet*>& s2){
	if(s1.empty() || s2.empty()){
		if (s1.empty()){
			return s2;
		}
		else if (s2.empty()){
			return s1;
		}
	}
	set<Tweet*> temp;
	set<Tweet*> :: iterator firstset;
	set<Tweet*> :: iterator secondset;
	for(firstset = s1.begin(); firstset != s1.end(); ++firstset){
		temp.insert(*firstset);
	}
	for(secondset = s2.begin(); secondset != s2.end(); ++secondset){
		temp.insert(*secondset);
	}

	return temp;
}

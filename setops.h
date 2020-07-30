//defines setops functions

#ifndef STR_H
#define STR_H

#include <set>
#include <string>
#include "tweet.h"

//set<Tweet*>

// Computes the intersection of s1 and s2
std::set<Tweet*> operator&(const std::set<Tweet*>& s1, 
                           const std::set<Tweet*>& s2);
// Computes the union of s1 and s2
std::set<Tweet*> operator|(const std::set<Tweet*>& s1, 
                           const std::set<Tweet*>& s2);

#endif
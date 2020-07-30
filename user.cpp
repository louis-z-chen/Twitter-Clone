//define user object functions

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <list>
#include "datetime.h"
#include "user.h"
#include "tweet.h"
#include "util.h"
#include "hsort.h"

using namespace std; 

//Constructor
User::User(std::string name){
	username_ = name;
}

//Destructor
// .clear deletes the pointers that are stored in those containers
User::~User(){
	followers_.clear();
	following_.clear();
	tweets_.clear();
}

//Gets the name of the user 
//return name of the user 
std::string User::name() const{
	return username_;
} 


//Gets all the followers of this user  
//return Set of Users who follow this user
std::set<User*> User::followers() const{
	return followers_;
}

//Gets all the users whom this user follows  
//return Set of Users whom this user follows
std::set<User*> User::following() const{
	return following_;
}

//Gets all the tweets this user has posted
//return List of tweets this user has posted
std::list<Tweet*> User::tweets() const{
	return tweets_;
} 

//Adds a follower to this users set of followers
//param u User to add as a follower
void User::addFollower(User* u){
	followers_.insert(u);
}

//Adds another user to the set whom this User follows
//param u User that the user will now follow
void User::addFollowing(User* u){
	following_.insert(u);
}

//Adds the given tweet as a post from this user
//@param t new Tweet posted by this user
void User::addTweet(Tweet* t){
	tweets_.push_back(t);
}

//Add the password hash 
void User::addPassword(unsigned int password_hash){
	password_hash_ = password_hash;
}

//Gets the password hash
unsigned int User::password() const{
	return password_hash_;
} 

/*
Produces the list of Tweets that represent this users feed/timeline
It should contain in timestamp order all the tweets from
this user and all the tweets from all the users whom this user follows

@return vector of pointers to all the tweets from this user
and those they follow in timestamp order
*/
std::vector<Tweet*> User::getFeed(){
	
	vector<Tweet*> temp;

	//adds this user's own tweets to the vector
	for (list<Tweet*>::iterator it = tweets_.begin(); it != tweets_.end(); ++it){
		temp.push_back(*it);
	}

	//adds this user's followers' tweets to the vector
	set<User*> followerset = following();
	for(set<User*>:: iterator it = followerset.begin(); it != followerset.end(); ++it){
		list<Tweet*> templist = (*it)->tweets();
		list<Tweet*>::iterator twit;
		for(twit = templist.begin(); twit != templist.end(); ++twit){
			string text = (*twit)->text();
			text = trim(text);
			if(text.empty() == false){
				stringstream ss(text);
				string firstword;		
				ss >> firstword;
				if(firstword[0] == '@'){
					firstword.erase(firstword.begin());
					if(firstword.empty() == false){			
						if(firstword == username_){
							if(check_follower((*twit)->name()) == true){
								temp.push_back(*twit);
							}
						}
					}
					else{
						temp.push_back(*twit);
					}
				}
				else{
					temp.push_back(*twit);
				}
			}
			else{
				temp.push_back(*twit);
			}
		}
	}

	//Sort the populated vector
	hsort(temp, TweetComp());

	return temp;
}

bool User::check_follower(std::string name){
	bool temp = false;
	
	//followers_ is the set of users following them
	//following_ is the set of users whom he/she follows

	User* tempname = nullptr;
	set<User*>::iterator it;
	for(it = followers_.begin(); it != followers_.end(); ++it){
		if((*it)->name() == name){
			temp = true;
			tempname = *it;
		}
	}

	if(temp == false){
		return temp;
	}
	
	set<User*> afollowers = tempname->followers();
	set<User*>::iterator setit;
	for(setit = afollowers.begin(); setit != afollowers.end(); ++setit){
		if((*setit)->name() == username_){
			temp = true;
		}
	}
	
	return temp;
}
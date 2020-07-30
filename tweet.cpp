//defines the tweet class functions

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include "datetime.h"
#include "user.h"
#include "tweet.h"
#include "util.h"

using namespace std; 

//Default constructor
Tweet::Tweet(){
}

//Constructor
//all the hashtags of a tweet are automatically added
//everytime a tweet is constructed
Tweet::Tweet(User* user, const DateTime& time, const std::string& text){
	timestamp_ = time;
	user_ = user;			
	text_ = text;
	add_hashtag();
	add_mentions();
}

//Gets the timestamp of the tweet
DateTime const & Tweet::time() const{
	return timestamp_;
}

//Gets the actual text of the tweet
std::string const & Tweet::text() const{
	return text_;
}

//Returns the hashtagged words without the '#' sign
std::set<std::string> Tweet::hashTags() const{
	return hashtags_;
}

//return true if this tweet's timestamp is less-than other's
//return result of less-than comparison of tweet's timestamp
bool Tweet::operator<(const Tweet& other) const{
	bool temp = false;
	if(timestamp_ < other.timestamp_){
		temp = true;
	}
	return temp;
} 

//outputs the tweet to the given ostream in format:
//YYYY-MM-DD HH:MM:SS username tweet_text
//return the ostream passed in as an argument
//setfill and setw make sure the format is the same everytime
std::ostream& operator<<(std::ostream& os, const Tweet& t){
	
	//get year
	os << setfill('0') << setw(4) << t.timestamp_.year << "-";

	//get month
	os << setfill('0') << setw(2) << t.timestamp_.month << "-";

	//get day
	os << setfill('0') << setw(2) << t.timestamp_.day << " ";

	//get hour
	os << setfill('0') << setw(2) << t.timestamp_.hour << ":";

	//get min
	os << setfill('0') << setw(2) << t.timestamp_.min << ":";

	//get sec
	os << setfill('0') << setw(2) << t.timestamp_.sec << " ";

	//username
	os << t.getusername() << " ";

	//tweet_text
	os << t.text_;

	return os;
}

//returns the user object who posted this tweet
User* Tweet::user() const{
	return user_;
}

//helper function to add hashtags to a tweet object
void Tweet::add_hashtag(){
	if (text_ != " "){
		stringstream ss(text_);
		string tempword;
		while (ss >> tempword){
			if(tempword[0] == '#'){
				tempword.erase(tempword.begin());
				if(tempword.empty() == false){
					convLower(tempword);
					hashtags_.insert(tempword);
				}
			}
		}
	}
}

//helper function to get username string from object name
std::string const Tweet::getusername() const{
	string temp;
	temp = user_->name();
	return temp;
}

//Returns the mentioned words without the '#' sign
std::set<std::string> Tweet::mentions() const{
	return mentions_;
}

//helper function to add mentions to a tweet object
void Tweet::add_mentions(){
	if (text_ != " "){
		if(text_[0]=='@'){
			stringstream ss(text_);
			string firstmention;
			ss >> firstmention;
			if(firstmention[0]== '@'){
				firstmention.erase(firstmention.begin());
				if(firstmention.empty() == false){
					mentions_.insert(firstmention);
				}
			}
		}
		else{
			stringstream ss(text_);
			string tempword;
			while (ss >> tempword){
				if(tempword[0] == '@'){
					tempword.erase(tempword.begin());
					if(tempword.empty() == false){
						mentions_.insert(tempword);
					}
				}
			}
		}
	}
}

std::string Tweet::name() const{
	return user_->name();
}
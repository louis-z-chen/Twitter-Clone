#include <map>
#include <string>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "tweet.h"
#include "user.h"
#include "datetime.h"
#include "twiteng.h"
#include "setops.h"
#include "util.h"
#include "hsort.h"
#include "stack.h"
#include "heap.h"
#include "tagdata.h"
#include "hash.h"

using namespace std;

//default constructor
TwitEng::TwitEng() : trending_(2)
{

}

//destructor
//iterates through every tweet of every user and deletes every
//tweet object and user object
TwitEng::~TwitEng(){
    map<string,User*>::iterator it;
    for(it = twittermap.begin(); it != twittermap.end(); ++it){
        User* currentuser = it -> second;
        list<Tweet*> templist = currentuser -> tweets();

        //iterating through list of tweet pointers
        list<Tweet*>::iterator lit;
        for(lit = templist.begin(); lit != templist.end(); ++lit){
            Tweet* temptweet = *lit;
            delete temptweet;
        }
        delete currentuser;
    }
}

//Parses the Twitter database and populates internal structures
//@param filename of the database file
//@return true if there is an error, false if successful
bool TwitEng::parse(char* filename){

    //read in input file
    ifstream ifile(filename);
    if(ifile.fail()){
    	return true;
    }

    //read in number of users
    int numusers;
    string num;
    getline(ifile, num);

    //if first line is empty
    //check notes at line 81
    while(num.empty()){
        return true;
        getline(ifile, num);
    }

    //read in number of users
    stringstream a(trim(num));
    a >> numusers;
    if(a.fail()){
    	return true;
    }

    //create user objects
    for(int i = 0; i < numusers; i++){
        string wholeline;
    	getline(ifile, wholeline);

        //this section has a while loop and a getline because it was 
        //originally intended to make the program work with spaces 
        //in the names section but the assignment considers this case 
        //invalid, thus returning true
        if(wholeline.empty()){
            return true;
        }

        stringstream ss(trim(wholeline));

        string currentuser;
        ss >> currentuser;
        if(ss.fail()){
            return true;
        }

        unsigned int password_hash;
        ss >> password_hash;
        if(ss.fail()){
            return true;
        }

        if(twittermap.find(currentuser) == twittermap.end()){
            User* ptr = new User(currentuser);
            ptr -> addPassword(password_hash);
            twittermap.insert({currentuser, ptr});
        }
        else{
            map<string,User*>::iterator it = twittermap.find(currentuser);
            User* tempuser = it -> second;
            tempuser -> addPassword(password_hash);
        }
        
        string name;
        queue<string> following_usernames;
        while(ss >> name){
            //add name to twittermap if not in alreadly
            name = trim(name);
            if(twittermap.find(name) == twittermap.end()){
                User *ptr = new User(name);
                twittermap.insert({name, ptr});
            }

            //add to queue
            following_usernames.push(name);

            //iterating through the queue
            while (!following_usernames.empty()){
                //make sure someone doesn't follow themselves
                if(following_usernames.front() == currentuser){
                    following_usernames.pop();
                }
                else{
                    User* current = twittermap.find(currentuser) -> second;
                    User* following = twittermap.find(following_usernames.front())-> second;
                    current->addFollowing(following);
                    following->addFollower(current);
                    following_usernames.pop();
                }
            }
        }
    }

    //add tweets to the user objects
    string tweetline;
    while(getline(ifile,tweetline)){
        //if the line is blank get the next line
        if(checkblank(trim(tweetline))){
            continue;
        }

        string reference = tweetline;

        //get datetime
        stringstream ss(trim(tweetline));
        DateTime tempdt;
        ss >> tempdt;

    	//get username
    	string tempname;
    	ss >> tempname;
        if (ss.fail()){
            return true;
        }
        trim(tempname);
        
    	//get text
    	string text;
    	getline(ss,text);
        trim(text);

    	//add tweet to user
        if(twittermap.find(tempname) != twittermap.end()){
            //calls the twiteng add tweet function which calls the
            //tweet class add tweet function
            addTweet(tempname, tempdt, text);
        }
        else{
            return true;
        }
    }

    //create map of hashtags and mentions
    //iterating through map of user pointers
    map<string,User*>::iterator it;
    for(it = twittermap.begin(); it != twittermap.end(); ++it){
    	User* currentuser = it -> second;
    	list<Tweet*> templist = currentuser -> tweets();

    	//iterating through list of tweet pointers
    	list<Tweet*>::iterator lit;
    	for(lit = templist.begin(); lit != templist.end(); ++lit){
    		Tweet* temptweet = *lit;

    		//iterating through set of hashtag strings
            //if the hashtag doesn't exist in the hashtag map
            //add it to the hashtag map with the tweet object pointer
            set<string> temphash = temptweet -> hashTags();
    		set<string>::iterator sit;
    		for(sit = temphash.begin(); sit != temphash.end(); ++sit){
    			if(hashmap.find(*sit) == hashmap.end()){
    				set<Tweet*> emptyset;
    				hashmap.insert({*sit, emptyset});
    			}
    			hashmap[*sit].insert(temptweet);
    		}

            //iterating through set of mention strings
            //if the mention doesn't exist in the mention map
            //add it to the mention map with the tweet object pointer
            set<string> tempmention = temptweet -> mentions();
            set<string>::iterator setit;
            for(setit = tempmention.begin(); setit != tempmention.end(); ++setit){
                if(twittermap.find(*setit) != twittermap.end()){
                    if(mentionmap.find(*setit) == mentionmap.end()){
                        set<Tweet*> emptyset;
                        mentionmap.insert({*setit, emptyset});
                    }
                    mentionmap[*setit].insert(temptweet);
                }
            }
    	}
    }
    return false;
}

//Allocates a tweet from its parts and adds it to internal structures
//@param username of the user who made the tweet
//@param timestamp of the tweet
//@param text is the actual text of the tweet as a single string
void TwitEng::addTweet(const std::string& username, const DateTime& time, const std::string& text){

    User* tempuser = twittermap.find(username) -> second;
    Tweet *ptr = new Tweet(tempuser,time,text);
    tempuser->addTweet(ptr);

    //get the hashtags in the new tweet
    set<string> temphash = ptr -> hashTags();
    set<string>::iterator it;
    for(it = temphash.begin(); it != temphash.end(); ++it){

        string name = *it;
        int old_num = 0;
        int new_num = 0;
        bool was_in_heap = false;

        if(hashmap.find(*it) != hashmap.end()){
            was_in_heap = true;
            old_num = ((hashmap.find(*it)) -> second).size();
        }
        new_num = old_num + 1;

        TagData old(name, old_num);
        TagData new_struct(name, new_num);

        if(was_in_heap){
            trending_.decreaseKey(old, new_struct);
        }
        else{
            trending_.push(new_struct);
        }
    }

    //add the new hashtags to the hashmap
    set<string>::iterator sit;
    for(sit = temphash.begin(); sit != temphash.end(); ++sit){
        if(hashmap.find(*sit) == hashmap.end()){
            set<Tweet*> emptyset;
            hashmap.insert({*sit, emptyset});
        }
        hashmap[*sit].insert(ptr);
    }

    set<string> tempmention = ptr -> mentions();
    set<string>::iterator setit;
    for(setit = tempmention.begin(); setit != tempmention.end(); ++setit){
        if(twittermap.find(*setit) != twittermap.end()){
            if(mentionmap.find(*setit) == mentionmap.end()){
                set<Tweet*> emptyset;
                mentionmap.insert({*setit, emptyset});
            }
            mentionmap[*setit].insert(ptr);
        }
    }
}

//Searches for tweets with the given words and strategy
//@param words is the hashtag terms in any case without the '#'
//@param strategy 0=AND, 1=OR
//@return the tweets that match the search
std::vector<Tweet*> TwitEng::search(std::vector<std::string>& terms, int strategy){
	
    if (terms.empty() == true){
        vector<Tweet*> temp{};
        return temp;
    }

    set<Tweet*> temp;
	temp = hashmap[terms[0]];
	for(unsigned int i = 0; i < terms.size() - 1; i++){
		if(strategy == 0){
			temp = temp & hashmap[terms[i+1]];
		}
		else if(strategy == 1){
			temp = temp | hashmap[terms[i+1]];
		}
	}
	//make vector
	vector<Tweet*> final(temp.size());
	copy(temp.begin(),temp.end(),final.begin());

	if(final.size() > 1){
       hsort(final, TweetComp());
    }
	return final;
}

//Dump feeds of each user to their own file
void TwitEng::dumpFeeds(){

    //dump main feed
	map<string,User*>::iterator it;
    for(it = twittermap.begin(); it != twittermap.end(); ++it){
    	User* currentuser = it -> second;
    	string username = currentuser-> name();
    	string filename = username;
    	string feed = ".feed";
    	filename.append(feed);
    	ofstream ofile(filename);
        if(ofile.is_open() == true){
    	   ofile << username << endl;
    	   for (unsigned int i = 0; i < currentuser->getFeed().size(); i++){
    	   	   ofile << *(currentuser->getFeed()[i]) << endl;
    	   }
        }
    	ofile.close();
    }

    //dump mention feed	
    map<string,User*>::iterator mapit;
    for(mapit = twittermap.begin(); mapit != twittermap.end(); ++mapit){
        User* currentuser = mapit -> second;
        string username = currentuser-> name();
        string filename = username;
        string feed = ".mentions";
        filename.append(feed);
        ofstream ofile(filename);
        if(ofile.is_open() == true){
            ofile << username << endl;
            for (unsigned int i = 0; i < getMentions(username).size(); i++){
                ofile << *(getMentions(username)[i]) << endl;
            }
        }
        ofile.close();
    }
}

//helper function for the Tweet handler to check if a 
//user exists in the twittermap
bool TwitEng::findusername(std::string username){
    bool temp = true;
    if(twittermap.find(username) == twittermap.end()){
        temp = false;
    }
    return temp;
}

//return true if the line is blank
bool TwitEng::checkblank(std::string tweetline){
    bool temp = true;
    for(unsigned int i = 0; i < tweetline.size(); i++){
        if(isblank(tweetline[i]) == false){
            temp = false;
            break;
        }
    }
    return temp;
}

//helper function for the follow command
void TwitEng::follow(std::string name_of_follower, std::string name_to_follow){

    //if name of follower isn't in map create new user
    if(twittermap.find(name_of_follower) == twittermap.end()){
        User *ptr = new User(name_of_follower);
        twittermap.insert({name_of_follower,ptr});
    }

    User* follower = twittermap.find(name_of_follower)->second;
    User* to_follow = twittermap.find(name_to_follow)->second;
    follower->addFollowing(to_follow);
    to_follow->addFollower(follower);
}

//helper function for save command
//return true if save failed, false if successful
bool TwitEng::save(std::string filename){
    ofstream ofile(filename);
    if(ofile.is_open() == false){
        return true;
    }

    int numusers = twittermap.size();

    //output number of users
    ofile << numusers << endl;

    //output username and their followers
    map<string,User*>::iterator it;
    for(it = twittermap.begin(); it != twittermap.end(); ++it){
        ofile << it->first << " ";
        User* currentuser = it -> second;
        unsigned int passhash = currentuser -> password();
        ofile << passhash << " ";
        set<User*> tempset = currentuser -> following();
        set<User*>::iterator setit;
        for(setit = tempset.begin(); setit != tempset.end(); ++setit){
            ofile << (*setit) -> name() << " ";
        }
        ofile << endl;
    }

    //output tweets
    map<string,User*>::iterator mapit;
    for(mapit = twittermap.begin(); mapit != twittermap.end(); ++mapit){
        User* current = mapit -> second;
        list<Tweet*> templist = current -> tweets();
        list<Tweet*>::iterator listit;
        for(listit = templist.begin(); listit != templist.end(); ++listit){
            ofile << *(*listit) << endl;
        }
    }
    ofile.close();
    return false;
}

//get mentions feed
std::vector<Tweet*> TwitEng::getMentions(std::string name){

    vector<Tweet*> temp;

    if(mentionmap.find(name) != mentionmap.end()){
        map<string,set<Tweet*>>::iterator mapit;
        mapit = mentionmap.find(name);
        set<Tweet*> mentionset = mapit->second;
        set<Tweet*>::iterator setit;
        for(setit = mentionset.begin(); setit != mentionset.end(); ++setit){
            temp.push_back(*setit);
        }
    }

    //Sort the populated vector
    hsort(temp, TweetComp());

    return temp;
}

//helper function for save command
//tarjan's algorithm
//return true if save failed, false if successful
bool TwitEng::find_scc(std::string filename){
    
    //check if outfile is open
    ofstream ofile(filename);
    if(ofile.is_open() == false){
        return true;
    }

    //declare variables
    //id_count is used to assign id values to nodes
    //everytime an id is assigned id_count is incremented
    int unvisited = -1;
    int num_nodes = twittermap.size();
    int id_count = 0;
    int scc_count = 0;

    //intialize list of users
    vector<User*> users;
    map<string,User*>::iterator it;
    for(it = twittermap.begin(); it != twittermap.end(); ++it){
        User* currentuser = it -> second;
        users.push_back(currentuser);
    }

    //create adjacency list
    vector<vector<User*>> adjacency= create_adjacency(users);

    //intialize arrays
    vector<int> node_ids(num_nodes,unvisited);
    vector<int> node_lowlink(num_nodes,unvisited);
    vector<bool> on_stack(num_nodes,false);

    //all the vector indexes corresepond to the same nodes, index 0
    //of users is the name of the node, adjacency[0] is a vector 
    //of people the node follows, node_ids is the id of the node,
    //and node_lowlink is the lowlink value of the same node;

    //intialize stack
    Stack<User*> tarjan_stack;

    //call dfs on every unvisited node
    for(int i = 0; i < num_nodes; i++){
        if(node_ids[i] == unvisited){
            tarjan_dfs(i, unvisited, scc_count, id_count, ofile, node_ids,
                                                    node_lowlink,
                                                    on_stack,
                                                    tarjan_stack,
                                                    users,
                                                    adjacency);
        }
    }

    ofile.close();
    return false;
}

//dfs function for tarjan
void TwitEng::tarjan_dfs(int index, int& unvisited, int& scc_count, int& id_count,
                                            std::ofstream& filename,
                                            std::vector<int>& node_ids,
                                            std::vector<int>& node_lowlink,
                                            std::vector<bool>& on_stack, 
                                            Stack<User*>& stack,
                                            std::vector<User*>& users,
                                            std::vector<std::vector<User*>>& adjacency){
    //add the node to the stack to signify visited
    //and set id, lowlink, and change to true if on_stack
    //set the depth index to the smallest unused index
    stack.push(users[index]);
    on_stack[index] = true;
    node_ids[index] = node_lowlink[index] = id_count++;
    
    //check neighbor
    for(unsigned int i = 0; i < adjacency[index].size(); i++){
        int neighbor_index = find_index(adjacency[index][i], users);

        //visit all neighbours and min lowlink on callback
        //call dfs on neighbor if unvisited
        if(node_ids[neighbor_index] == unvisited){
            tarjan_dfs(neighbor_index, unvisited, scc_count, id_count, filename, node_ids,
                        node_lowlink, on_stack, stack, users, adjacency);
            node_lowlink[index] = min(node_lowlink[index],node_lowlink[neighbor_index]);
        }
        //if the neighbor was alreadly visited
        //and is on the stack min the lowlink values
        else if(on_stack[neighbor_index] == true){
            node_lowlink[index] = min(node_lowlink[index],node_lowlink[neighbor_index]);
        }
    }
    //after visiting all the neighbours, check if it is the start
    //of scc. Pop the stack until we're at the start of the scc
    if(node_ids[index] == node_lowlink[index]){
        if(scc_count > 0){
            filename << endl;
        }
        filename << "Component " << scc_count + 1 << endl;
        //while there are nodes in the stack
        //add to current scc
        while(stack.empty() == false){
            User* temp = stack.top();
            stack.pop();
            filename << temp -> name() << endl;
            int temp_index = find_index(temp, users);
            on_stack[temp_index] = false;
            node_lowlink[temp_index] = node_ids[index];

            if(index == temp_index){
                break;
            }
        }
        scc_count++;
    }
}

//create adjacency list
std::vector<std::vector<User*>> TwitEng::create_adjacency(std::vector<User*> users){

    vector<vector<User*>> finalvector;
    int num_users = twittermap.size();
    for(int i = 0; i < num_users; i++){
        vector<User*> temp;
        set<User*> following = users[i] -> following();
        set<User*>::iterator it;
        for(it = following.begin(); it != following.end(); ++it){
            temp.push_back(*it);
        }
        finalvector.push_back(temp);
    }
    return finalvector;
}

//helper function for tarjan dfs
//given user* find index
int TwitEng::find_index(User* ptrname, vector<User*>& users){
    int temp = -1;
    for(unsigned int i = 0; i < users.size(); i++){
        if(users[i] == ptrname){
            temp = i;
        }
    }
    return temp;
}

//helper function for trending command
//return true if failed, false if successful
bool TwitEng::trending(int num){
    
    vector<TagData> temp;
    int total_hashtags = hashmap.size();

    int num_to_output = num;

    if(num > total_hashtags){
        num_to_output = total_hashtags;
    }

    for(int i = 0; i < num_to_output; i++){
        temp.push_back(trending_.top());
        trending_.pop();
    }

    for(unsigned int i = 0; i < temp.size(); i++){
        cout << temp[i].tag << " : " << temp[i].num << endl;
        trending_.push(temp[i]);
    }

    return false;
}


//helper function for login command
//return true if failed, false if successful
bool TwitEng::login(std::string username, std::string password){
    
    unsigned int passhash = calculateHash(password);

    map<string, User*>::iterator it;
    it = twittermap.find(username);
    if(it == twittermap.end()){
        return true;
    }

    User* login_user = it -> second;
    unsigned int actual_pass_hash = login_user -> password();

    if(passhash == actual_pass_hash){
        current_user_ = login_user;
    }
    else{
        return true;
    }
    return false;
}

//helper function for logout command
//return true if failed, false if successful
bool TwitEng::logout(){

    current_user_ = nullptr;
    return true;
}

//function to see if someone is logged in
bool TwitEng::is_logged_in(){
    bool temp = true;
    if(current_user_ == nullptr){
        temp = false;
    }
    return temp;
}

//function to get the currently logged in user's name
std::string TwitEng::get_user(){
    string temp;

    if(current_user_ != nullptr){
        temp = current_user_ -> name();
    }

    return temp;
}
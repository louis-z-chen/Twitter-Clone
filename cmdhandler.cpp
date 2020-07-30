//defines the functions for the handler child classes 

#include <iostream>
#include <string>
#include "cmdhandler.h"
#include "util.h"
#include "twiteng.h"
#include "datetime.h"
using namespace std;


////////////////////////////Quit Handler////////////////////////////
//handles the quit command
QuitHandler::QuitHandler(){

}

QuitHandler::QuitHandler(Handler* next)
  : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const{
	return cmd == "QUIT";

}

Handler::HANDLER_STATUS_T QuitHandler::process(TwitEng* eng, std::istream& instr) const{
	eng->dumpFeeds();
	return HANDLER_QUIT;
}

////////////////////////////And Handler////////////////////////////
//handles the and command
AndHandler::AndHandler(){

}

AndHandler::AndHandler(Handler* next)
  : Handler(next)
{

}

bool AndHandler::canHandle(const std::string& cmd) const{
	return cmd == "AND";
}

Handler::HANDLER_STATUS_T AndHandler::process(TwitEng* eng, std::istream& instr) const{
	vector<string> terms;
	string temp;
	//put lowercased search terms in vector
	while (instr >> temp){
		convLower(temp);
		terms.push_back(temp);
	}

	if(terms.size() == 0){
		return HANDLER_ERROR;
	}

	vector<Tweet*> hits = eng->search(terms,0);
	displayHits(hits);
	return HANDLER_OK;
}

////////////////////////////Or Handler////////////////////////////
//handles the or command
OrHandler::OrHandler(){

}

OrHandler::OrHandler(Handler* next)
  : Handler(next)
{

}

bool OrHandler::canHandle(const std::string& cmd) const{
	return cmd == "OR";
}

Handler::HANDLER_STATUS_T OrHandler::process(TwitEng* eng, std::istream& instr) const{
	vector<string> terms;
	string temp;
	//put lowercased search terms in vector
	while (instr >> temp){
		convLower(temp);
		terms.push_back(temp);
	}

	if(terms.size() == 0){
		return HANDLER_ERROR;
	}

	vector<Tweet*> hits = eng->search(terms,1);
	displayHits(hits);
	return HANDLER_OK;
}

////////////////////////////Tweet Handler////////////////////////////
//handles the tweet command
TweetHandler::TweetHandler(){

}

TweetHandler::TweetHandler(Handler* next)
  : Handler(next)
{

}

bool TweetHandler::canHandle(const std::string& cmd) const{
	return cmd == "TWEET";
}

Handler::HANDLER_STATUS_T TweetHandler::process(TwitEng* eng, std::istream& instr) const{
	
	if(eng->is_logged_in() == false){
		std::cout << "No user logged in" << std::endl;
		return HANDLER_ERROR;
	}

	string username;
	username = eng->get_user();
	if(username.empty()){
		return HANDLER_ERROR;
	}

	//gets trimmed text
	string text;
	getline(instr,text);
	trim(text);
	DateTime current;
	eng->addTweet(username,current,text);
	return HANDLER_OK;
}

////////////////////////////Follow Handler////////////////////////////
//handles the follow command
FollowHandler::FollowHandler(){

}

FollowHandler::FollowHandler(Handler* next)
  : Handler(next)
{

}

bool FollowHandler::canHandle(const std::string& cmd) const{
	return cmd == "FOLLOW";
}

Handler::HANDLER_STATUS_T FollowHandler::process(TwitEng* eng, std::istream& instr) const{
	
	if(eng->is_logged_in() == false){
		std::cout << "No user logged in" << std::endl;
		return HANDLER_ERROR;
	}

	string name_of_follower;
	name_of_follower = eng->get_user();
	if(name_of_follower.empty()){
		return HANDLER_ERROR;
	}

	string name_to_follow;
	instr >> name_to_follow;
	if(instr.fail()){
		return HANDLER_ERROR;
	}

	trim(name_of_follower);
	trim(name_to_follow);

	if(name_of_follower == name_to_follow){
		return HANDLER_ERROR;
	}

	//if name_to_follow doesn't exist in map, it's an error
	if(eng->findusername(name_to_follow) == false){
		return HANDLER_ERROR;
	}

	//if name_to_follow doesn't exist in map, it's an error
	if(eng->findusername(name_of_follower) == false){
		return HANDLER_ERROR;
	}

	eng->follow(name_of_follower, name_to_follow);
	return HANDLER_OK;
}

////////////////////////////Save Handler////////////////////////////
//handles the save command
SaveHandler::SaveHandler(){

}

SaveHandler::SaveHandler(Handler* next)
  : Handler(next)
{

}

bool SaveHandler::canHandle(const std::string& cmd) const{
	return cmd == "SAVE";
}

Handler::HANDLER_STATUS_T SaveHandler::process(TwitEng* eng, std::istream& instr) const{
	string output_file;
	instr >> output_file;
	if(instr.fail()){
		return HANDLER_ERROR;
	}

	if(eng->save(output_file)){
		return HANDLER_ERROR;
	}
	return HANDLER_OK;
}

////////////////////////////SCC Handler////////////////////////////
//handles the scc command
SccHandler::SccHandler(){

}

SccHandler::SccHandler(Handler* next)
  : Handler(next)
{

}

bool SccHandler::canHandle(const std::string& cmd) const{
	return cmd == "SCC";
}

Handler::HANDLER_STATUS_T SccHandler::process(TwitEng* eng, std::istream& instr) const{
	string output_file;
	instr >> output_file;
	if(instr.fail()){
		return HANDLER_ERROR;
	}

	if(eng->find_scc(output_file)){
		return HANDLER_ERROR;
	}
	return HANDLER_OK;
}

////////////////////////////Trending Handler////////////////////////////
//handles the trending command
TrendingHandler::TrendingHandler(){

}

TrendingHandler::TrendingHandler(Handler* next)
  : Handler(next)
{

}

bool TrendingHandler::canHandle(const std::string& cmd) const{
	return cmd == "TRENDING";
}

Handler::HANDLER_STATUS_T TrendingHandler::process(TwitEng* eng, std::istream& instr) const{
	
	int num;
	instr >> num;
	if(instr.fail()){
		return HANDLER_ERROR;
	}
	if(num < 0){
		return HANDLER_ERROR;
	}
	else if(num == 0){
		return HANDLER_OK;
	}

	if(eng->trending(num)){
		return HANDLER_ERROR;
	}
	return HANDLER_OK;
}

////////////////////////////Login Handler////////////////////////////
//handles the login command
LoginHandler::LoginHandler(){

}

LoginHandler::LoginHandler(Handler* next)
  : Handler(next)
{

}

bool LoginHandler::canHandle(const std::string& cmd) const{
	return cmd == "LOGIN";
}

Handler::HANDLER_STATUS_T LoginHandler::process(TwitEng* eng, std::istream& instr) const{

	bool temp = true;
	string username;
	string password;

	instr >> username;
	if(instr.fail()){
		temp = false;
		return HANDLER_ERROR;
	}

	instr >> password;
	if(instr.fail()){
		temp = false;
		return HANDLER_ERROR;
	}
	password = trim(password);

	if(eng->findusername(username) == false){
		temp = false;
	}

	if(password.size() > 8){
		temp = false;
	}

	if(temp == false){
		std::cout << "Invalid username/password." << std::endl;
		return HANDLER_ERROR;
	}

	temp = (eng->login(username, password));

	if(temp){
		std::cout << "Invalid username/password." << std::endl;
		return HANDLER_ERROR;
	}
	else{
		std::cout << "Login successful." << std::endl;
		return HANDLER_OK;
	}
	return HANDLER_OK;
}

////////////////////////////Logout Handler////////////////////////////
//handles the Logout command
LogoutHandler::LogoutHandler(){

}

LogoutHandler::LogoutHandler(Handler* next)
  : Handler(next)
{

}

bool LogoutHandler::canHandle(const std::string& cmd) const{
	return cmd == "LOGOUT";
}

Handler::HANDLER_STATUS_T LogoutHandler::process(TwitEng* eng, std::istream& instr) const{

	if(eng->is_logged_in()){
		if(eng->logout()){
			return HANDLER_ERROR;
		}
	}
	return HANDLER_OK;
}
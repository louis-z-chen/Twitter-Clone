//declares the twiteng class

#ifndef TWITENG_H
#define TWITENG_H
#include <map>
#include <string>
#include <set>
#include <vector>
#include "stack.h"
#include "user.h"
#include "datetime.h"
#include "heap.h"
#include "tagdata.h"

class TwitEng
{
 public:
  TwitEng();
  ~TwitEng();
  /**
   * Parses the Twitter database and populates internal structures
   * @param filename of the database file
   * @return true if there is an error, false if successful
   */
  bool parse(char* filename);

  /**
   * Allocates a tweet from its parts and adds it to internal structures
   * @param username of the user who made the tweet
   * @param timestamp of the tweet
   * @param text is the actual text of the tweet as a single string
   */
  void addTweet(const std::string& username, const DateTime& time, const std::string& text);

  /**
   * Searches for tweets with the given words and strategy
   * @param words is the hashtag terms in any case without the '#'
   * @param strategy 0=AND, 1=OR
   * @return the tweets that match the search
   */
  std::vector<Tweet*> search(std::vector<std::string>& terms, int strategy);

  /**
   * Dump feeds of each user to their own file
   */
  void dumpFeeds();

  /* You may add other member functions */

  //helper function for the Tweet handler to check if a 
  //user exists in the twittermap
  bool findusername(std::string username);

  //check if a line is blank
  bool checkblank(std::string tweet);

  //helper function for the follow command
  void follow(std::string name_of_follower, std::string name_to_follow);

  //helper function for save command
  bool save(std::string filename);

  //helper function for save command
  //tarjan's algorithm
  bool find_scc(std::string filename);

  //helper function for trending command
  bool trending(int num);

  //helper function for Login command
  bool login(std::string username, std::string password);

  //helper function for Logout command
  bool logout();

  //function to see if someone is logged in
  bool is_logged_in();

  //function to get the currently logged in user's name
  std::string get_user();

 private:
  /* Add any other data members or helper functions here  */

  //map for the users where the key is the username(string) and 
  //the element is the user object pointer
  std::map<std::string,User*> twittermap;

  //map for the hashtags where the key is the hashtag(string) and
  //the element is the tweet object pointer
  std::map<std::string,std::set<Tweet*>> hashmap;

  //map for the mentions where the key is the mentioned name(string) and
  //the element is the tweet object pointer
  std::map<std::string,std::set<Tweet*>> mentionmap;

  //get mentions feed
  std::vector<Tweet*> getMentions(std::string name);

  //create adjacency list
  std::vector<std::vector<User*>> create_adjacency(std::vector<User*> users);

  //helper dfs function for tarjan
  void tarjan_dfs(int index, int& unvisited, int& scc_count, int& id_count,
                                      std::ofstream& filename,
                                      std::vector<int>& node_ids,
                                      std::vector<int>& node_lowlink,
                                      std::vector<bool>& on_stack, 
                                      Stack<User*>& stack,
                                      std::vector<User*>& users,
                                      std::vector<std::vector<User*>>& adjacency);

  //helper function for tarjan dfs
  //given user* find index
  int find_index(User* ptrname, std::vector<User*>& users);

  //trending heap
  Heap<TagData, TagStringEqual, TagIntGreater, TagStringHasher> trending_;

  //current loged in user
  User* current_user_ = nullptr;

};


#endif

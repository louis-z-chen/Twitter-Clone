//defines global function

#include <string>
#include <cctype>
#include "util.h"
#include "hsort.h"

using namespace std;

void convLower(string& s)
{
  for(unsigned int i=0; i < s.size(); i++){
    s[i] = tolower(s[i]);
  }
}

void convUpper(string& s)
{
  for(unsigned int i=0; i < s.size(); i++){
    s[i] = toupper(s[i]);
  }
}

void displayHits(vector<Tweet*>& hits)
{
  if(hits.size() > 0){
    hsort(hits, TweetComp());
    cout << hits.size() << " matches:" << endl;
    for(vector<Tweet*>::iterator it = hits.begin(); it != hits.end(); ++it){
      cout << (**it) << endl;
    }
  }
  else {
    cout << "No matches." << endl;
  }
}

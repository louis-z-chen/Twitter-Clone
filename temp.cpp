    //create user objects
    bool firstname = true;
    string wholeline;
    for(int i = 0; i < numusers; i++){
    	getline(ifile, wholeline);

        //this section has a while loop and a getline because it was 
        //originally intended to make the program work with spaces 
        //in the names section but the assignment considers this case 
        //invalid, thus returning true
        if(wholeline.empty()){
            return true;
        }

        //iterates through all the names in a line and if they don't
        //exist add them as a user object
        //I have a queue of people I should add to the following list
        //it would iterate through the queue after all the user
        //objects are made
    	string name;
    	string currentuser;
    	stringstream ss(trim(wholeline));
    	queue<string> following_usernames;
    	while(ss >> name){
            trim(name);
    		if(twittermap.find(name) == twittermap.end()){
    			User *ptr = new User(name);
    			twittermap.insert({name, ptr});
    		}
    		if(firstname == false){
    			following_usernames.push(name);
    		}
    		else{
                //save the first name so the queue knows 
                //who to add to the following list
    			currentuser = name;
    		}
    		firstname = false;
    	}
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
    	firstname = true;
    }
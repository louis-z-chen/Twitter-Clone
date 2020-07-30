CC       = g++
CPPFLAGS = -g -Wall -std=c++11
GTEST_LL = -I /usr/include/gtest/ -l gtest -l gtest_main -pthread

All: twitter heap_tester

twitter: twitter.cpp twiteng.o cmdhandler.o handler.o user.o tweet.o datetime.o util.o setops.o hash.o
	$(CC) $(CPPFLAGS) $^ -o twitter

twiteng.o: twiteng.cpp twiteng.h tweet.h user.h datetime.h setops.h util.h hsort.h stack.h hash.h
	$(CC) $(CPPFLAGS) -c $< -o twiteng.o

cmdhandler.o: cmdhandler.cpp cmdhandler.h util.h twiteng.h datetime.h
	$(CC) $(CPPFLAGS) -c $< -o cmdhandler.o
	
user.o: user.cpp user.h datetime.h tweet.h hsort.h
	$(CC) $(CPPFLAGS) -c $< -o user.o

tweet.o: tweet.cpp tweet.h datetime.h user.h util.h
	$(CC) $(CPPFLAGS) -c $< -o tweet.o

datetime.o: datetime.cpp datetime.h
	$(CC) $(CPPFLAGS) -c $< -o datetime.o

util.o: util.cpp util.h hsort.h
	$(CC) $(CPPFLAGS) -c $< -o util.o

handler.o: handler.cpp handler.h
	$(CC) $(CPPFLAGS) -c $< -o handler.o

setops.o: setops.cpp setops.h tweet.h
	$(CC) $(CPPFLAGS) -c $< -o setops.o

hash.o: hash.cpp hash.h
	$(CC) $(CPPFLAGS) -c $< -o hash.o

heap_tester: heap_tester.cpp heap.h
	$(CC) $(CPPFLAGS) $^ -o heap_tester

.PHONY: clean clean_tests

clean:
	rm -f *.o *~ *.feed *.mentions *.out twitter heap_tester

clean_tests:
	rm -f *~ *.feed *.mentions *.out
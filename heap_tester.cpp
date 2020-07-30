#include <iostream>
#include "heap.h"

using namespace std; 

int main(){
	
	/*
	Heap<int> heap(2);
	heap.push(0);
	heap.push(5);
	heap.push(10);
	heap.push(15);
	heap.push(20);
	heap.decreaseKey(0,0);
	heap.decreaseKey(5,4);
	heap.decreaseKey(10,9);
	heap.decreaseKey(15,14);
	heap.decreaseKey(20,19);
	
	cout << heap.top() << endl;
	heap.pop();
	cout << heap.top() << endl;
	heap.pop();
	cout << heap.top() << endl;
	heap.pop();
	cout << heap.top() << endl;
	heap.pop();
	cout << heap.top() << endl;
	heap.pop();
	*/

	Heap<int> heap(2);
	heap.push(40);
	heap.push(30);
	heap.push(20);
	heap.push(10);

	//EXPECT_TRUE(checkMapCorrelation(heap));
	//EXPECT_EQ(10, heap.top());
	//cout << heap.top() << endl;
	heap.decreaseKey(40, 35);
	
	//EXPECT_TRUE(checkMapCorrelation(heap));
	//EXPECT_EQ(10, heap.top());
	cout << heap.top() << endl;
	heap.pop();

	//EXPECT_TRUE(checkMapCorrelation(heap));
	//EXPECT_EQ(20, heap.top());
	cout << heap.top() << endl;
	heap.pop();

	//EXPECT_TRUE(checkMapCorrelation(heap));
	//EXPECT_EQ(30, heap.top());
	cout << heap.top() << endl;
	heap.pop();

	//EXPECT_TRUE(checkMapCorrelation(heap));
	//EXPECT_EQ(35, heap.top());
	cout << heap.top() << endl;

	return 0;
}
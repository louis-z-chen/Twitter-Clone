#ifndef HSORT_H
#define HSORT_H

#include <vector>

// heapify() helper function
// loc - Location to start the heapify() process
// effsize - Effective size (number of items in the vector that
// are part of the heap). Useful for performing heap-sort in place.
template <typename T, typename Comparator >
void heapify(std::vector<T>& data, size_t loc, size_t effsize, Comparator& c ){
	
	//if vector is size 0 or 1
	if(data.size() == 0 || data.size() == 1){
		return;
	}

	if(loc >= data.size() || loc >= effsize){
		return;
	}

	if(effsize > data.size()){
		return;
	}

	//root and child nodes
	size_t root = loc;
	size_t left = (2*root)+1;
	size_t right = (2*root)+2;
	size_t smaller_index = 0; 

	//if it's a leaf node
	if(left >= effsize && right >= effsize){
		return;
	}

	//compare left and right child
	if(left < effsize && right < effsize){
		if(c(data[left],data[right])){
			smaller_index = left;
		}
		else{
			smaller_index = right;
		}
	}
	//if left exists and right doesn't
	else if(left < effsize && right >= effsize){
		smaller_index = left;
	}
	//compare smaller index with current
	if(c(data[root],data[smaller_index])){
		smaller_index = root;
	}
	//if the smaller index is not current, swap
	if(smaller_index != root){
		T temp = data[root];
		data[root] = data[smaller_index];
		data[smaller_index] = temp;
		heapify(data,smaller_index,effsize,c);
	}
}

template <typename T, typename Comparator >
void hsort(std::vector<T>& data, Comparator c = Comparator()){

	//if vector is size 0 or 1
	if(data.size() == 0 || data.size() == 1){
		return;
	}

	//call heapify on first non-leaf and move up
	size_t numnode = data.size();
	for(size_t i = numnode; i != 0; i--){
		size_t left = (2*(i-1))+1;
		size_t right = (2*(i-1))+2;

		if(left < numnode || right < numnode){
			heapify(data,i-1,numnode,c);
		}
	}

	//topping and popping
	size_t heap_size = numnode;
	while(heap_size > 1){
		//swap first and last
		T temp = data[0];
		data[0] = data[heap_size-1];
		data[heap_size-1] = temp;

		//heapify root
		heapify(data,0,heap_size-1,c);

		//shrink effective size
		heap_size--;
	}

	//reverse array
	//linear time
	size_t start_index = 0;
	size_t end_index = numnode-1;
	while(start_index < end_index){
		T swap = data[start_index];
		data[start_index] = data[end_index];
		data[end_index] = swap;
		start_index++;
		end_index--;
	}
}

#endif
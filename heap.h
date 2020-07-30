#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <iostream>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
template <
         typename T,
         typename KComparator = std::equal_to<T>,
         typename PComparator = std::less<T>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const KComparator& kcomp = KComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item
    void push(const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey - Finds key matching old object &
    /// updates its location in the heap based on the new value
    void decreaseKey(const T& old, const T& newVal);

 private:
    /// Add whatever helper functions and data members you need below
    void swap(int index1, int index2);
    void trickle_up(int index);
    void trickle_down(int index);

 private:
   /// Data members - These should be sufficient
    std::vector< T > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, KComparator> keyToLocation_;

};

// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
Heap<T,KComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const KComparator& kcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, kcomp)
{

}

// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
Heap<T,KComparator,PComparator,Hasher>::~Heap()
{

}

// Incomplete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::push( const T& item){
    store_.push_back(item);
    trickle_up(store_.size()-1);
}

// Incomplete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::decreaseKey(const T& old, const T& newVal){

    //only peform function if the new value is "better"
    if(c_(newVal, old)){
        typename std::unordered_map<T, size_t, Hasher, KComparator>::iterator it = 
                                        keyToLocation_.find(old);
        int index = it -> second;
        //std::cout << index << std::endl;
        store_[index] = newVal;
        trickle_up(index);
    }
}

// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
T const & Heap<T,KComparator,PComparator,Hasher>::top() const{

    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }
    // If we get here we know the heap has at least 1 item
    // Add code to return the top element
    return store_[0];
}

// Incomplete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::pop(){

    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    keyToLocation_.erase(store_[0]);
    store_[0]=store_[store_.size()-1];
    store_.pop_back();
    trickle_down(0);
}

/// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
bool Heap<T,KComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}

//Swap Function
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::swap(int index1, int index2){
    T temp = store_[index1];
    store_[index1] = store_[index2];
    store_[index2] = temp;

    keyToLocation_[store_[index1]] = size_t(index1);
    keyToLocation_[store_[index2]] = size_t(index2);
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::trickle_up(int index){

    if(index == 0){
        keyToLocation_[store_[index]] = size_t(index);
        return;
    }
    int temp_index = index;
    int parent = (temp_index-1)/m_;
    if(parent >= 0 && c_(store_[temp_index],store_[parent])){
        swap(temp_index,parent);
        trickle_up(parent);
    }
    else{
        keyToLocation_[store_[index]] = size_t(index);
    }
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::trickle_down(int index){

    //if vector is size 0
    if(store_.size() == 0){
        return;
    }
    //if vector is size 0 or 1
    if(store_.size() == 1){
        keyToLocation_[store_[index]] = size_t(index);
        return;
    }

    //find all valid child nodes
    std::vector<int> child_nodes;
    for(int i = 0; i < m_; i++){
        int temp = m_*index + (i+1);
        if(temp < int(store_.size())){
            child_nodes.push_back(temp);
        }
        else{
            break;
        }
    }
    //if index is a leaf node
    if(child_nodes.empty() == true){
        keyToLocation_[store_[index]] = size_t(index);
        return;
    }

    //find smallest child nodes
    int smallest_index;
    if(child_nodes.size()==1){
        smallest_index = child_nodes[0];
    }
    else{
        smallest_index = child_nodes[0];
        for(size_t i = 1; i < child_nodes.size(); i++){
            if(c_(store_[child_nodes[i]],store_[smallest_index])){
                smallest_index = child_nodes[i];
            }
        }
    }
    //now smallest_index should have the index of the
    //smallest_index of the current node

    //if the smallest index is not node
    if(c_(store_[smallest_index],store_[index])){
        swap(smallest_index, index);
        trickle_down(smallest_index);
    }
    else{
        keyToLocation_[store_[index]] = size_t(index);
    }
}

#endif


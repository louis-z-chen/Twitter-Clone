#ifndef DEQUE_H
#define DEQUE_H
#include <stdexcept>

/**
 * Implements a templated, array-based, 
 * double-ended queue.
 *
 * The Deque should allow for amortized O(1) 
 * insertion and removal from either end. 
 * It should dynamically resize, when it becomes
 * full, but never needs to shrink.  
 *
 * It should use a circular buffer approach
 * by tracking the "front" and "back" location.
 *
 */


template <typename T>
class Deque
{
 public:
  /* START - DO NOT CHANGE ANY OF THESE LINES */
  /**
   *  Default constructor 
   *   Create an array of size 1 by default
   */
  Deque();

  /**
   *  Default constructor 
   *   Create an array of the given capacity.
   */
  Deque(size_t capacity);

  /**
   *  Destructor
   */
  ~Deque();

  /**
   *  Returns the number of elements in the Deque. 
   *   - Must run in O(1)
   */
  size_t size() const;

  /**
   *  returns true if the deque is empty
   *   - Must run in O(1)
   */
  bool empty() const;

  /**
   *  returns  the i-th element from the front
   *   - Must run in O(1)
   *   - Must throw std::range_error if i is
   *      an invalid location
   */
  T& operator[](size_t i);

  /**
   *  returns  the i-th element from the front
   *   - Must run in O(1)
   *   - Must throw std::range_error if i is
   *      an invalid location
   */
  T const & operator[](size_t i) const;

  /**
   *  Inserts a new item at the back of the
   *  Deque
   *   - Must run in amortized O(1)
   */
  void push_back(const T& item);

  /**
   *  Inserts a new item at the front of the
   *  Deque
   *   - Must run in amortized O(1)
   */
  void push_front(const T& item);

  /**
   *  Removes the back item from the Deque
   *   - Must run in amortized O(1)
   *   - Simply return if the deque is empty
   */
  void pop_back();

  /**
   *  Removes the front item from the Deque
   *   - Must run in amortized O(1)
   *   - Simply return if the deque is empty
   */
  void pop_front();
  /* END - DO NOT CHANGE ANY OF THESE LINES */
 private:
  /* Add data members and private helper 
   * functions below 
   */
  size_t head_;  //head points to the first element
  size_t tail_;  //head points to next available space
  size_t size_;
  size_t capacity_;
  T* data_;

  //helper function to check if full
  bool full() const;

  //helper function to expand array
  void expand();

};

/* Implement each Deque<T> member function below 
 * taking care to meet the runtime requirements
 */

/**
*  Default constructor 
*   Create an array of size 1 by default
*/
template <typename T>
Deque<T>::Deque(){
    head_ = 0;
    tail_ = 0;
    size_ = 0;
    capacity_ = 1;
    data_ = new T[1];
}

/**
*  Default constructor 
*   Create an array of the given capacity.
*/
template <typename T>
Deque<T>::Deque(size_t capacity){
    head_ = 0;
    tail_ = 0;
    size_ = 0;
    capacity_ = capacity;
    data_ = new T[capacity];
}

/**
*  Destructor
*/
template <typename T>
Deque<T>::~Deque(){
    delete[] data_;
}

/**
*  Returns the number of elements in the Deque. 
*   - Must run in O(1)
*/
template <typename T>
size_t Deque<T>::size() const{
    return size_;
}

/**
*  returns true if the deque is empty
*   - Must run in O(1)
*/
template <typename T>
bool Deque<T>::empty() const{
    bool temp = false;
    if(size_ == 0){
      temp = true;
    }
    return temp;
}

/**
*  returns  the i-th element from the front
*   - Must run in O(1)
*   - Must throw std::range_error if i is
*      an invalid location
*/
template <typename T>
T& Deque<T>::operator[](size_t i){
    if(i >= size_ || i < 0){
      throw std::range_error("range_error");
    }
    return data_[(head_ + i) % capacity_];
}

/**
*  returns  the i-th element from the front
*   - Must run in O(1)
*   - Must throw std::range_error if i is
*      an invalid location
*/
template <typename T>
T const & Deque<T>::operator[](size_t i) const{
    if(i >= size_ || i < 0){
      throw std::range_error("range_error");
    }
    return data_[(head_ + i) % capacity_];
}


/**
*  Inserts a new item at the back of the
*  Deque
*   - Must run in amortized O(1)
*/
template <typename T>
void Deque<T>::push_back(const T& item){
    if(full()){
        expand();
    }
    data_[tail_] = item;
    tail_ = (tail_+ 1) % capacity_;
    size_++;
}

/**
*  Inserts a new item at the front of the
*  Deque
*   - Must run in amortized O(1)
*/
template <typename T>
void Deque<T>::push_front(const T& item){
    if(full()){
        expand();
    }
    if(head_ == 0){
        head_ = capacity_ - 1;
    }
    else{
        head_ = (head_ - 1) % capacity_;
    }
    data_[head_] = item;
    size_++;
}

/**
*  Removes the back item from the Deque
*   - Must run in amortized O(1)
*   - Simply return if the deque is empty
*/
template <typename T>
void Deque<T>::pop_back(){
    if(empty()){
        return;
    }
    tail_ = (tail_- 1) % capacity_;
    size_--;
}

/**
*  Removes the front item from the Deque
*   - Must run in amortized O(1)
*   - Simply return if the deque is empty
*/
template <typename T>
void Deque<T>::pop_front(){
    if(empty()){
        return;
    }
    head_ = (head_ + 1) % capacity_;
    size_--;
}

template <typename T>
bool Deque<T>::full() const{
    bool temp = false;
    if(size_ >= capacity_){
        temp = true;
    }
    return temp;
}

template <typename T>
void Deque<T>::expand(){
    T* ptr = new T[3*capacity_];
    for(unsigned int i = 0; i < size_; i++){
        ptr[i] = data_[(head_ + i) % capacity_];
    }
    delete[] data_;
    data_ = ptr;
    head_ = 0;
    tail_ = size_;
    capacity_ = 3 * capacity_;
}

#endif
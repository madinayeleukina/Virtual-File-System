#include <iostream>
#include <string>
#include <fstream>
using namespace std;

template <typename T>
class MyQueue{
private:
	T *array;
	int n; //number of current elements in the Queue
	int N; //array size (capacity)
	int f; //index of front element
	int r; //index where a new element will be added
public:
	MyQueue(int N); //constructor
	~MyQueue(); //destructor
	void makeEmpty(); //make queue empty
	void push(T); //add element to queue
	T pop(); //remove first element of queue
	T& front() const{ //get first element of queue
		if (n!=0) return array[f];
		else throw "Queue is empty";
	};
	bool empty() const { return n==0; } //check is queue empty
};

#include "MyQueue.cpp"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

template <typename T> 
class MyVector{
private:
    T* arr; //pointer to int(array) to store elements
    int capacity; //capacity of vector
    int sz; //current size of vector (number of elements in vector)
public:

    MyVector(){ //constructor
        arr = new T[1];
        capacity = 1;
        sz = 0;
    }
    T& operator[](int ind) { //return reference to the element at given index
        if (ind<sz) return arr[ind];
        else throw out_of_range("Index is out of range");
    }
    int size() { return sz; } //return vector size
    bool empty() { return sz==0; } //check is vector empty
    void push_back(T data){ //add element to the end
        if (sz==capacity) {
            T* temp = new T[2 * capacity];
            for (int i = 0; i < capacity; i++) {
                temp[i] = arr[i];
            }
            delete[] arr;
            capacity *= 2;
            arr = temp;
        }
        arr[sz] = data;
        sz++;
    }
    void erase(int ind) { //erase element at given index
    	for (int i=ind;i<sz-1;i++) arr[i] = arr[i+1];
    	sz--;
    }
};
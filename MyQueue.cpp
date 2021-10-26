template<typename T>
MyQueue<T>::MyQueue(int N) {
	this->N = N;
	array = new T[N];
	n = 0;
	r = 0;
	f = 0;
}

template<typename T>
MyQueue<T>::~MyQueue() {
	delete[] this->array;
}

template<typename T>
void MyQueue<T>::makeEmpty() {
	delete[] array;
	array = new T[N];
	n = 0;
	r = 0;
	f = 0;
}

template<typename T>
T MyQueue<T>::pop() {
	if (n==0) throw "Queue is empty";
	T val = array[f];
	f = (f+1) % N; 
	n--;
	return val;
}

template<typename T>
void MyQueue<T>::push(T elem) {
	if (n==N) throw "Queue is full";
	array[r] = elem;
	r = (r+1) % N;
	n++;
}
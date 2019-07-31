#include <iostream>
#include <deque>
#include <functional>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <memory>
#include <thread>

// plain enumeration definition
enum Colour { white, red, blue };
// scoped enumeration definition
enum class Pet { cat, dog, fish };
// class template with default values
template<typename T = int, int size = 50>
class Array {
public:
	T b[size];
	T* a = nullptr;
	unsigned n;
	T foo;
	Array() : n(0), foo(0) {}
	// FUNCTOR
	void operator()(){}
	// copy constructor
	Array(const Array& src) { *this = src; }
	// move-constructor
	Array(Array&& src) { *this = std::move(src); }
	// copy assignment operator
	Array& operator=(const Array& src) {
		if (this != &src) {
			delete[] a;
			a = new int[src.n];
			for (unsigned i = 0u; i < src.n; ++i)
				a[i] = src.a[i];
			n = src.n;
			b[0] = src.b[0], b[1] = src.b[1];
		}
		return *this;
	}
	// move assignment operator
	Array& operator=(Array&& src) {
		if (this != &src) {
			delete[] a;
			a = src.a;
			n = src.n;
			foo = src.foo;
			b[0] = src.b[0], b[1] = src.b[1];
			src.b[0] = 0, src.b[1] = 0;
			src.a = nullptr;
			src.n = 0u;
			src.foo = 0;
		}
		return *this;
	}
	T& operator[](unsigned i) {
		return i < size ? b[i] : foo;
	}
	void push_back(T num) {
		if (n < size) {
			b[n] = num;
			n++;
		}
	}
};


// function template
template <typename T>
bool maximum(T a, T b) {
	return a > b;
}

// pointer to function
template <class T>
void sort(Array<>& a, int n, bool (*max)(T, T)) {
	for (int i = n - 1; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			if (max(a[j], a[j + 1])) {
				T temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
		}
	}
}
template <> // denotes specialization
bool maximum<char*>(char* a, char* b) {
	return std::strcmp(a, b) > 0;
}
template <typename T>
void print(const T& t) {
	std::cout << t << std::endl;
}
// variadic template
template <typename T, typename... etc>
void print(const T& t, const etc& ... pp) {
	std::cout << t << ", ";
	print(pp...);
}
// trailing return type deceleration 
template<typename T, typename U>//need auto and ->
auto add(const T& t, const U& u) -> decltype(t + u)
{
	return t + u;
}
// template to process lambda
template<typename Func>
int add(int i, Func func) { return func(i); }


//START SECOND HALF SEMESTER
std::ofstream fout("output.txt"); // text access
std::ofstream fout("output.txt", std::ios::out | 
	std::ios::binary | std::ios::trunc); //binary


int main() {
	//START SECOND HALF SEMESTER
	std::deque<std::thread> Arrays;
	Arrays.push_back(std::thread(std::unique_ptr<Array<int>>
		(new Array<int>())));
	Arrays[0].join();
	Colour wall = white;
	Pet myPets[] = { Pet::cat };
	Array <> a;
	a.push_back(53);
	a.push_back(52);
	sort(a, a.n, maximum<int>);
	std::cout << a[0] << std::endl; // 52
	Array<> b = std::move(a);
	std::cout << a[0] << std::endl; // 0
	print("Adam", 100, 99.99); // call variadic
	int k = 4;
	// pass lambda to add function
	std::cout << add(10, [&](int i) { return i + k++; })
		<< std::endl; // 14
	std::cout << "k = " << k << std::endl;
	// k = 5
	// size is 3 all values 19.35
	std::deque<double> d(3, 19.35);
	d.back() = 33.22;   // reset last 
	d.erase(d.begin()); // remove first
	for (auto i = d.begin(); i != d.end(); i++)
		std::cout << *i << "  "; // 19.35 33.22
	std::cout << std::endl;
	d.insert(++d.begin(), 122.123);
	d.insert(d.end(), 19.25);
	// i is std::deque<double>::iterator
	for (auto i = d.begin(); i != d.end(); i++)
		// 19.35 122.123 33.22 19.25
		std::cout << *i << "  ";
	std::cout << std::endl;
}


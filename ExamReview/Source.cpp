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
	void operator()() {}
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

int main() {
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

	// START SECOND HALF: FUNCTIONAL
	std::function<int(int, int)> fu = add<int, int>;
	std::cout << "fu(1,1) = " << fu(1, 1); // 2
	std::cout << std::endl;
	int i = 2; // fu2 is reference to i
	std::reference_wrapper<int> fu2 = i;
	auto p = std::bind(add<int, int>, 1, 1);
	std::cout << p() << std::endl; // 2

	// ALGORITHM
	int n = std::count(d.begin(),d.end(), 19.35);
	std::cout << n << std::endl; // 1
	n = std::count_if(d.begin(), d.end(), 
		[](int i) {return !(i & 1); });
	std::cout << n << std::endl; // 1
	std::deque<double> e(4);
	// copy d's values into e
	std::copy(d.begin(), d.end(), e.begin());
	// range based for - STL container!!!
	for (auto i : e)
		// 19.35 122.123 33.22 19.25
		std::cout << i << " ";
	std::cout << std::endl;
	std::deque<double> ef(1);
	// copy d's even number into ef
	std::copy_if(d.begin(), d.end(), ef.begin(),
		[](int i) {return !(i % 2); });
	std::deque<double> g(1);
	std::transform(ef.begin(), ef.end(), g.begin(),
		[](int i) {return --i; }); //g[0] = 121
	std::sort(d.begin(), d.end());
	for (auto i : d)
		// 19.25 19.35 33.22 122.123
		std::cout << i << " ";
	std::cout << std::endl;

	//NUMERIC
	double ac = std::accumulate(d.begin(), d.end(), 0);
	//accumulate the products of each pair in the range
	ac = std::inner_product(g.begin(), g.end(), 
		ef.begin(), (double)0);
	//go through container and add next element to previous
	//sum - insert into e
	e = std::deque<double>(4);
	d = std::deque<double>(4, 1);
	std::partial_sum(d.begin(), d.end(), e.begin());
	for (auto i : e)
		// 1 2 3 4
		std::cout << i << " ";
	std::cout << std::endl;

	// std::ios::trunc default
	std::ofstream fout("text.txt"); // text access
	// std::ios::app default
	std::fstream f("output.txt", std::ios::out | std::ios::in |
		std::ios::binary); // binary
	f.seekp(0, std::ios::beg);// start of the file
	f.seekp(5, std::ios::cur);// 5 bytes beyond current
	std::streampos pos = f.tellp();// current position
	f.seekp(0, std::ios::end);// end of the file
	int streamsize = f.tellp();
	f.seekp(0);
	const char* s = new char[streamsize];
	f.read(const_cast<char*>(s), streamsize);
	f.write(s, streamsize);
	f.seekp(0);
	std::unique_ptr<Array<>> up(new Array<>());
	std::cout << up->foo;// 0
	std::thread t1(fu, 1, 1);
	t1.join();
	std::deque<std::thread> threads;
	threads.push_back(std::thread(fu, 1, 1));
	threads[0].get_id();
	std::this_thread::get_id();
	threads[0].join();
	return 0;
}



#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <iostream>
#include <string>

#include "vector.hpp"

namespace sam {


struct block {
	block (const char* s) { std::cout << s << '\n'; }
	~block() { std::cout << "\n<<< ------\n\n"; }
};

#define BLOCK_NAME_HELPER(name, num) name ## num
#define BLOCK_NAME(num) BLOCK_NAME_HELPER(block_, num)

#define BLOCK(s) \
	sam::block BLOCK_NAME(__LINE__) {"--- " s " --->>>\n"};

// dump_name

inline void dump_name(const std::string& var)
{
	std::cout << var;
}

// dump size & capacity

template <typename T>
inline void dump_sz_cap(const vector<T>& v) {
	std::cout << '(' << v.size() << ", " << v.capacity() << ')';
};

#define DUMP_SZ_CAP(var) \
	dump_name(#var); \
	dump_sz_cap(var); \
	std::cout << '\n';

// dump elements

template <typename T>
void dump_elements(const vector<T>& v)
{
	using namespace std;

	cout << "{ ";
	for (typename vector<T>::size_type i = 0; i < v.size(); ++i) {
		cout << v[i];
		if (i != v.size() - 1)
			cout << ", ";
	}
	cout << " }";
}

#define DUMP_ELEMENTS(var) \
	dump_name(#var); \
	dump_elements(var); \
	std::cout << '\n';

// dump size, capacity & elements

#define DUMP(var) \
	dump_name(#var); \
	dump_sz_cap(var); \
	cout << ' '; \
	dump_elements(var); \
	std::cout << '\n';


struct box {
	int val;
	static unsigned count;

	explicit box (int v)
		:val{v}
	{
		++count;
	}

	box(const box& o): val {o.val}
	{
		++count;
	}

	box& operator = (const box& o)
	{
		++count;
		if (this == &o) return *this;
		val = o.val;
		return *this;
	}

	~box () {
		--count;
	}
};

unsigned box::count{0};

std::ostream& operator << (std::ostream& os, const box& b)
{
	return os << b.val;
}


} // namespace sam


#endif // _UTIL_HPP


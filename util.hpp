
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

inline void dump_sz_cap(const vector& v) {
	std::cout << '(' << v.size() << ", " << v.capacity() << ')';
};

// TOTO: remove
//inline void dump_sz_cap(const std::string& varname, const vector& v) {
//	std::cout << varname;
//	dump_sz_cap(v);
//};

#define DUMP_SZ_CAP(var) \
	dump_name(#var); \
	dump_sz_cap(var); \
	std::cout << '\n';

// dump elements

void dump_elements(const vector& v)
{
	using namespace std;

	cout << "{ ";
	for (vector::size_type i = 0; i < v.size(); ++i) {
		cout << v[i];
		if (i != v.size() - 1)
			cout << ", ";
	}
	cout << " }";
}

//void dump_elements(const std::string& varname, const vector& v) {
//	std::cout << varname;
//	dump_elements(v);
//}

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

} // namespace sam


#endif // _UTIL_HPP


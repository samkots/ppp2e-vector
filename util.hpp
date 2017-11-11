
#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <iostream>

namespace sam {


struct block {
	block (const char* s) { std::cout << s << '\n'; }
	~block() { std::cout << "\n<<< ------\n\n"; }
};

#define BLOCK(s) \
	sam::block b_ ## _LINE_ {"--- " s " --->>>\n"};


} // namespace sam


#endif // _UTIL_HPP


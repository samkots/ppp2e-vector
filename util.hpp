
#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <iostream>

namespace sam {


struct block {
	block (const char* s) { std::cout << s << '\n'; }
	~block() { std::cout << "\n<<< ------\n\n"; }
};

#define BLOCK_NAME_HELPER(name, num) name ## num
#define BLOCK_NAME(num) BLOCK_NAME_HELPER(block_, num)

#define BLOCK(s) \
	sam::block BLOCK_NAME(__LINE__) {"--- " s " --->>>\n"};


} // namespace sam


#endif // _UTIL_HPP


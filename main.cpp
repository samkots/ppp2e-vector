
#include <iostream>
#include <stdexcept>
#include "vector.hpp"

using namespace std;


int main() try
{
	cout << "Hello, World!\n";
}
catch (const bad_alloc& x) {
	cerr << "error: " << x.what() << '\n';
	return 3;
}
catch (const exception& x) {
	cerr << "error: " << x.what() << '\n';
	return 2;
}
catch (...) {
	cerr << "error!\n";
	return 1;
}


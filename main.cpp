
#include <iostream>
#include <stdexcept>
#include "vector.hpp"
#include "util.hpp"

using namespace std;
using namespace sam;


int main() try
{
	using vst = vector::size_type;

	{ BLOCK("ctor v, vv(4)")
		vector v;
		cout << "v.szie: " << v.size() << endl;
		cout << "v.capacity: " << v.capacity() << endl;

		vector vv(4);
		cout << "vv.szie: " << vv.size() << endl;
		cout << "vv.capacity: " << vv.capacity() << endl;

		for (vst i = 0; i < vv.size(); ++i) cout << vv[i] << '\n';
	}

	{ BLOCK("subscript v(4)")
		vector v(4);
		cout << "v[2]: " << v[2] << endl;
		cout << "v.at(2): " << v.at(2) << endl;

		//cout << "v[4]: " << v[4] << endl;
		//cout << "v.at(4): " << v.at(4) << endl;
	}
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


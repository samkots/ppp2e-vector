
#include <iostream>
#include <stdexcept>
#include "vector.hpp"
#include "util.hpp"

using namespace std;
using namespace sam;


int main() try
{
	{ BLOCK("ctor v, vv(4)")
		vector v;
		DUMP_SZ_CAP(v);

		vector vv(4);
		DUMP(vv);
	}

	{ BLOCK("subscript v(4)")
		vector v(4);
		cout << "v[2]: " << v[2] << endl;
		cout << "v.at(2): " << v.at(2) << endl;

		//cout << "v[4]: " << v[4] << endl;
		//cout << "v.at(4): " << v.at(4) << endl;
	}

	{ BLOCK("reserve v, vv(4)")
		vector v, vv(4);
		DUMP_SZ_CAP(v)
		DUMP_SZ_CAP(vv)

		cout << "\n----- 0, 2\n\n";

		v.reserve(0);
		vv.reserve(2);
		DUMP_SZ_CAP(v)
		DUMP_SZ_CAP(vv)

		cout << "\n----- 4, 6\n\n";

		v.reserve(4);
		vv.reserve(6);
		DUMP_SZ_CAP(v)
		DUMP_SZ_CAP(vv)
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



#include <iostream>
#include <stdexcept>
#include "vector.hpp"
#include "util.hpp"

using namespace std;
using namespace sam;


int main() try
{
	// vector-of-double's test cases...

	//{ BLOCK("ctor v, vv(4)")
	//	vector v;
	//	DUMP_SZ_CAP(v);

	//	vector vv(4);
	//	DUMP(vv);
	//}

	//{ BLOCK("subscript v(4)")
	//	vector v(4);
	//	cout << "v[2]: " << v[2] << endl;
	//	cout << "v.at(2): " << v.at(2) << endl;

	//	//cout << "v[4]: " << v[4] << endl;
	//	//cout << "v.at(4): " << v.at(4) << endl;
	//}

	//{ BLOCK("reserve v, vv(4)")
	//	vector v, vv(4);
	//	DUMP_SZ_CAP(v)
	//	DUMP_SZ_CAP(vv)

	//	cout << "\n----- 0, 2\n\n";

	//	v.reserve(0);
	//	vv.reserve(2);
	//	DUMP_SZ_CAP(v)
	//	DUMP_SZ_CAP(vv)

	//	cout << "\n----- 4, 6\n\n";

	//	v.reserve(4);
	//	vv.reserve(6);
	//	DUMP_SZ_CAP(v)
	//	DUMP_SZ_CAP(vv)
	//}

	//{ BLOCK("resize() v, vv(4)")
	//	vector v, vv(4);
	//	DUMP(v)
	//	DUMP(vv)

	//	cout << "\n----- 0, 2\n\n";

	//	v.resize(0);			// case: sz == sz_
	//	vv.resize(2);			// case: sz < sz_
	//	DUMP(v)
	//	DUMP(vv)

	//	cout << "\n----- 4, 8\n\n";

	//	v.resize(4);			// case: sz > sz_
	//	vv.resize(8);
	//	DUMP(v)
	//	DUMP(vv)
	//}

	//{ BLOCK("push_back() v, vv(5)")
	//	vector v, vv(5);
	//	DUMP(v);
	//	DUMP(vv);

	//	cout << "\n----- 1.0, 2.0\n\n";

	//	v.push_back(1.0);		// case: sz_ == cap_
	//	vv.push_back(2.0);
	//	DUMP(v);
	//	DUMP(vv);

	//	cout << "\n----- 1.5, 2.5\n\n";

	//	v.push_back(1.5);		// case: sz_ < cap_
	//	vv.push_back(2.5);
	//	DUMP(v);
	//	DUMP(vv);
	//}

	// generalized vector test cases...

	{ BLOCK("<box>")
		vector<box> vb;
		DUMP(vb);
		vb.push_back(box{2});
		vb.push_back(box{4});
		vb.push_back(box{8});
		DUMP(vb);

		vector<box> vb2;
		DUMP(vb2);
		vb2 = vb;
		DUMP(vb2);

		vb.resize(0, box{0});
	}

	cout << "raii count: " << box::count << '\n';
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


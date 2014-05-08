// vim: noet ts=4 sw=4 sts=0
#include <iostream>
#include "variant.hh"
using namespace std;

namespace Abc
{
	void abc() {}
}

int main()
{
	Var v1;
	Var v2 = 100;
	Var v3 = true;
	Var v4 = 10.0;
	Var v5 = "12.33333333333333333333322111";
	cout << "VIBDS    values" << endl;
	cout << v1.is_void() << v1.is_int() << v1.is_bool() << v1.is_double() << v1.is_string()
		 << "    " << int(v1) << "," << bool(v1) << "," << double(v1) << "," << string(v1)<< endl;
	cout << v2.is_void() << v2.is_int() << v2.is_bool() << v2.is_double() << v2.is_string()
		 << "    " << int(v2) << "," << bool(v2) << "," << double(v2) << "," << string(v2)<< endl;
	cout << v3.is_void() << v3.is_int() << v3.is_bool() << v3.is_double() << v3.is_string()
		 << "    " << int(v3) << "," << bool(v3) << "," << double(v3) << "," << string(v3)<< endl;
	cout << v4.is_void() << v4.is_int() << v4.is_bool() << v4.is_double() << v4.is_string()
		 << "    " << int(v4) << "," << bool(v4) << "," << double(v4) << "," << string(v4)<< endl;
	cout << v5.is_void() << v5.is_int() << v5.is_bool() << v5.is_double() << v5.is_string()
		 << "    " << int(v5) << "," << bool(v5) << "," << double(v5) << "," << string(v5)<< endl;
	cout << endl;

	// string copy test
	{
		Var v6 = v5;
		cout << v6.is_void() << v6.is_int() << v6.is_bool() << v6.is_double() << v6.is_string()
			 << "    " << int(v6) << "," << bool(v6) << "," << double(v6) << "," << string(v6)<< endl;
	}
	cout << v5.is_void() << v5.is_int() << v5.is_bool() << v5.is_double() << v5.is_string()
		 << "    " << int(v5) << "," << bool(v5) << "," << double(v5) << "," << string(v5)<< endl;
	cout << endl;

	// non-string copy test
	{
		Var v6 = v3;
		cout << v6.is_void() << v6.is_int() << v6.is_bool() << v6.is_double() << v6.is_string()
			 << "    " << int(v6) << "," << bool(v6) << "," << double(v6) << "," << string(v6)<< endl;
	}
	cout << v3.is_void() << v3.is_int() << v3.is_bool() << v3.is_double() << v3.is_string()
		 << "    " << int(v3) << "," << bool(v3) << "," << double(v3) << "," << string(v3)<< endl;
	cout << endl;

	// stream test
	cout << v1 << endl;
	cout << v2 << endl;
	cout << v3 << endl;
	cout << v4 << endl;
	cout << v5 << endl;
}


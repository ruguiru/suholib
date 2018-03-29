#include "test_common.h"
#include "MyPacket.h"

#include <iostream>

using namespace std;

int main()
{
	MyPacket pack(1);
	pack << 2047 << (char)1 << std::string("abcd") << "efg";
	pack.ShowMemory();

	int a;
	char b;
	std::string c;
	char d[12] = {};

	pack >> a;
	pack >> b;
	pack >> c;
	pack >> d;

	cout << a;
	cout << b;
	cout << c;
	cout << d << endl;
}
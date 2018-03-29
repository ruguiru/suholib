
#include "test_common.h"

using namespace std;

class A
{
public:
    ~A()
    {
        cout << "destructor\n";
    }

	void Show()
	{
		cout << "Call Show\n";
	}

private:
    int a;
};


void foo()
{
    shared_ptr<A> p;
    p.reset(new A[10], default_delete<A[]>());
	p.get()[0].Show();

    unique_ptr<A[]> p2;
    p2.reset(new A[10]);
}

int main()
{
    foo();
}
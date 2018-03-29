
#include "test_common.h"
#include "atomic_shared_ptr.hpp"
#include "atomic_weak_ptr.hpp"

using namespace jss;




class MyClass
{
public:
    MyClass() { cout << "Ctor\n"; }
    MyClass(int a) { cout << "Ctor(int) " << a << "\n"; }
    ~MyClass() { cout << "Dtor\n"; }
};

int main()
{
    std::shared_ptr<int> sp;

    atomic_shared_ptr<int> sp2;
    atomic_shared_ptr<int> sp3 = std::make_shared<int>(1);
    atomic_shared_ptr<int> sp4(std::make_shared<int>(2));

    atomic_weak_ptr<int> wp = sp;
        
    //std::shared_ptr<int> sp = std::make_shared<int>(1);

    //atomic_shared_ptr<int> atomic_sptr(std::make_shared<int>(1));
}
#include "test_common.h"
#include "ObjectPool.hpp"

using namespace suho::pool;

struct Obj
{
public:
	Obj()
    {
        printf("Ctor1 Obj\n");
    }
	Obj(int v) :a(v)
    {
        printf("Ctor2 Obj\n");
    }
	int a = 1;
};

void main()
{
	//{
	//	ObjectPool<Obj, true> obj;
	//	obj.Create(1000);

	//	Obj* temp = obj.Borrow();
	//	temp->a;
	//	int index = obj.GetIndex(temp);

	//	Obj* temp2 = obj.Borrow();
	//	temp2->a;

	//	index = obj.GetIndex(temp2);

	//	obj.Return(temp);

	//	index = obj.GetIndex(temp2);

	//	Obj* temp3 = obj.Borrow();
	//	index = obj.GetIndex(temp3);
	//}

	{
	    //ManualObjectPool<Obj> obj;

	    //obj.Add(new Obj(1));
	    //obj.Add(new Obj(2));
	    //obj.Add(new Obj(3));
	    //obj.Add(new Obj(4));

        ManualObjectPool<Obj, true> obj;
        Obj* arr = new Obj[4];
        obj.SetArrayStartPtr(arr);
        new(arr + 0) Obj(1);            // placement new 사용하여 인자있는 생성자 호출 가능
        new(arr + 1) Obj(2);
        new(arr + 2) Obj(3);
        new(arr + 3) Obj(4);
        
        obj.Add(arr+0);
        obj.Add(arr+1);
        obj.Add(arr+2);
        obj.Add(arr+3);

	    Obj* temp = obj.Borrow();
	    int index = temp->a;

	    Obj* temp2 = obj.Borrow();
	    index = temp2->a;
	}
}
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
	{
		// 사용자타입 Obj, indexing 버전으로 설정
		// Obj 타입에 index 메모리가 붙게 되며 멤버함수 GetIndex()도 추가되어 확인가능하다
		ObjectPool<Obj, true> obj;
		obj.Create(1000);				// 최초 1000개 생성

		Obj* temp = obj.Borrow();		// Obj 한개 빌려옴
		temp->a;
		int index = obj.GetIndex(temp); // Index 값 확인

		obj.Return(temp);				// Obj 사용 후 반환

		Obj* temp2 = obj.Borrow();
		index = obj.GetIndex(temp2);
	}

	{
		// 외부 생성 후 직접 노드 삽입
	    ManualObjectPool<Obj> obj;

	    obj.Add(new Obj(1));
	    obj.Add(new Obj(2));
	    obj.Add(new Obj(3));
	    obj.Add(new Obj(4));

		// 배열로 생성 후 삽입도 가능
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
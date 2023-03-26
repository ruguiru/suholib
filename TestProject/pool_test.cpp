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
		// �����Ÿ�� Obj, indexing �������� ����
		// Obj Ÿ�Կ� index �޸𸮰� �ٰ� �Ǹ� ����Լ� GetIndex()�� �߰��Ǿ� Ȯ�ΰ����ϴ�
		ObjectPool<Obj, true> pool;
		pool.Create(1000);				// ���� 1000�� ����

		Obj* temp = pool.Borrow();		// Obj �Ѱ� ������
		temp->a;
		int index = pool.GetIndex(temp); // Index �� Ȯ��

		pool.Return(temp);				// Obj ��� �� ��ȯ

		Obj* temp2 = pool.Borrow();
		index = pool.GetIndex(temp2);
	}

	{
		// �ܺ� ���� �� ���� ��� ����
		ManualObjectPool<Obj> pool;

		pool.Add(new Obj(1));
		pool.Add(new Obj(2));
		pool.Add(new Obj(3));
		pool.Add(new Obj(4));
	}
	{
		// �迭�� ���� �� ���Ե� ����
        ManualObjectPool<Obj, true> pool;
        Obj* arr = new Obj[4];
        pool.SetArrayStartPtr(arr);
        new(arr + 0) Obj(1);            // placement new ����Ͽ� �����ִ� ������ ȣ�� ����
        new(arr + 1) Obj(2);
        new(arr + 2) Obj(3);
        new(arr + 3) Obj(4);
        
        pool.Add(arr+0);
        pool.Add(arr+1);
        pool.Add(arr+2);
        pool.Add(arr+3);

	    Obj* temp = pool.Borrow();
	    int index = temp->a;

	    Obj* temp2 = pool.Borrow();
	    index = temp2->a;
	}
}
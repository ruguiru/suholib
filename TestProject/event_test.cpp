
#include "test_common.h"
#include "EventSync.h"

#include <future>
#include <iostream>

using namespace std;
using namespace suho::thread;


suho::thread::EventSync  evt;

void th1()
{
	cout << "Enter th1\n";

	std::this_thread::sleep_for(2s);
	evt.Set();

	std::this_thread::sleep_for(2s);
	evt.Reset();

	std::this_thread::sleep_for(2s);
	evt.Set();

	std::this_thread::sleep_for(2s);
	evt.Reset();
}

void th2()
{
	cout << "Enter th2\n";

	while(true)
	{
		evt.Wait();

		cout << "After Wait\n";
	}	
}

int main()
{
	
	printf("before\n");
	auto f1 = std::async(th1);	// async 는 스코프 안에서만 비동기 스코프나가기전까지 get 안해주면 기다린다	
	

	printf("after\n");
	auto f2 = std::async(th2);

    f1.get();
    f2.get();
}
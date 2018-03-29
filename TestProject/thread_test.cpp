
#include <thread>
#include <functional>
#include <iostream>

#include "ThreadManager.hpp"

using namespace std;


int ThreadFunc1(int a, int b)
{
	this_thread::sleep_for(5s);

	cout << a << " " << b << "\n";

	return a + b;
}

int main()
{
	std::thread::id thread_id = suho::thread::ThreadManager::GetInstance()->Create(ThreadFunc1, 1, 2);

	//std::thread th1 = Spawn(ThreadFunc1, 1, 2);

	suho::thread::ThreadManager::GetInstance()->Join(thread_id);
}
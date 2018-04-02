
#include <thread>
#include <functional>
#include <iostream>

#include "ThreadManager.hpp"

using namespace std;


int ThreadFunc1(int a, int b)
{
	cout << a << " " << b << "\n";

	this_thread::sleep_for(5s);

	return a + b;
}

int main()
{
	std::thread::id thread_id = suho::thread::ThreadManager::GetInstance()->Create(ThreadFunc1, 1, 2);

	suho::thread::ThreadManager::GetInstance()->Join(thread_id);
}
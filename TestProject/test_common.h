#pragma once


#ifdef _DEBUG
#pragma comment(lib, "based.lib")
#pragma comment(lib, "winnetd.lib")
#else if
#pragma comment(lib, "base.lib")
#pragma comment(lib, "winnet.lib")
#endif

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#define SLEEP(x) std::chrono::seconds dura(x); std::this_thread::sleep_for(dura);
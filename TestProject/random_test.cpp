#include "test_common.h"
#include "Random.h"

void main()
{
	for (int i = 0; i < 100; i++)
	{
		std::cout << suho::util::random::GetNormal(50, 15) << std::endl;
	}

	for (int i = 0; i < 100; i++)
	{
		std::cout << suho::util::random::GetUniformInt(1, 10000) << std::endl;
	}
}
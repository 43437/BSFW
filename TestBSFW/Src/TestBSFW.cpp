#include <iostream>
#include "TestObj.h"

int main()
{
	auto pTestObj = new TestObj;

    std::cout << "Hello World!\n";

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}
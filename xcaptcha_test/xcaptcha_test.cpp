#include "pch.h"
#include <iostream>
#include <XCaptcha.h>

int main()
{
	try {
		XCaptcha x("Your key");

		//google demo
		auto task =  x.sendCaptcha("6Le-wvkSAAAAAPBMRTvw0Q4Muexq9bi0DJwx_mJ-", "https://www.google.com/recaptcha/api2/demo");

		//msvc sleep
		Sleep(25000);

		std::cout << x.getTaskResponse(task);
	}
	catch (const char* ex) {
		std::cout << ex;
	}
}


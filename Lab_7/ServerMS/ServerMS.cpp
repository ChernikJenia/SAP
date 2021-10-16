#include <iostream>
#include <windows.h>
#include <string>
#include "../MailslotErrors/framework.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "../ServerMS/debug/MailslotErrors.lib")

#define MAILSLOT_NAME TEXT("\\\\.\\mailslot\\BOX")
#define MAX_MESSAGE_LENGTH 300

int main()
{
	system("chcp 1251>nul");

	HANDLE sH;
	DWORD rb;

	try
	{
		if ((sH = CreateMailslot(
			MAILSLOT_NAME,
			MAX_MESSAGE_LENGTH,
			1000 * 60 * 3,
			NULL)) == INVALID_HANDLE_VALUE) {
			throw SetMailslotError("createMailslot: ", GetLastError());
		}

		std::cout << "Server is running..." << std::endl;

		char rbuf[MAX_MESSAGE_LENGTH]{};
		int msgCount = 0;
		clock_t  t1 = 0, t2 = 0;

		do 
		{
			if (!ReadFile(sH, rbuf, sizeof(rbuf), &rb, NULL)) {
				throw SetMailslotError("readFile: ", GetLastError());
			}
			else {
				if (msgCount == 0)
					t1 = clock();
			}

			msgCount++;

		} while (strlen(rbuf) > 0);

		t2 = clock();

		std::cout << std::endl << "duration: " << 
			((double)(t2 - t1)) / ((double)CLOCKS_PER_SEC) << "сек." << std::endl;

		std::cout << "Received " << msgCount << " messages" << std::endl;
	}
	catch (std::string ErrorMessage)
	{
		std::cout << ErrorMessage << std::endl;
	}

	system("pause");
	return 0;
}


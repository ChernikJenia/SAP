#include <iostream>
#include <windows.h>
#include <string>
#include "../MailslotErrors/framework.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "../ServerMS/debug/MailslotErrors.lib")



int main(int argc, char* argv[])
{
	system("chcp 1251>nul");

	HANDLE cH;
	DWORD wb;

	std::wstring server;
	std::wcout << L"Print name of server or domain, or * for primary domain, or . for localhost: ";
	std::getline(std::wcin, server);

	std::wstring mailslot;
	std::wcout << L"Print name of mailslot: ";
	std::getline(std::wcin, mailslot);

	std::wstring nameMailslot(L"\\\\");
	nameMailslot.append(server);
	nameMailslot.append(L"\\mailslot\\");
	nameMailslot.append(mailslot);
	std::wcout << L"Path to mailslot: " << nameMailslot << std::endl;


	try
	{
		if ((cH = CreateFile(nameMailslot.c_str(),
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			NULL,
			NULL)) == INVALID_HANDLE_VALUE) {
			throw SetMailslotError("createFile: ", GetLastError());
		}	

		char wbuf[] = "Hello from Mailslot-client";
		clock_t  t1 = 0, t2 = 0;

		t1 = clock();

		for (int i = 0; i < 1000; i++) {
			if (!WriteFile(cH, wbuf, sizeof(wbuf), &wb, NULL)) {
				throw SetMailslotError("writeFile: ", GetLastError());
			}
		}
		
		t2 = clock();

		std::cout << std::endl << "duration: " <<
			((double)(t2 - t1)) / ((double)CLOCKS_PER_SEC) << "сек." << std::endl;

		if (!WriteFile(cH, "", 1, &wb, NULL)) {
			throw SetMailslotError("writeFile: ", GetLastError());
		}

		CloseHandle(cH);

		std::cout << "Client's work has been done.." << std::endl;
	}
	catch (std::string errorMsgText)
	{
		std::cout << errorMsgText << std::endl;
	}

	system("pause");
	return 0;
}

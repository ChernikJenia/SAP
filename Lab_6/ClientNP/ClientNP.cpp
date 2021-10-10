#include <iostream>
#include <windows.h>
#include <string>
#include "../PipeError/framework.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "../ServerNP/debug/PipeError.lib")

int main(int argc, char* argv[])
{
	system("chcp 1251>nul");

	HANDLE cH;

	std::wstring server;
	std::wcout << L"Print name of server, or . for localhost: ";
	std::getline(std::wcin, server);

	std::wstring pipe;
	std::wcout << L"Print name of pipe: ";
	std::getline(std::wcin, pipe);

	std::wstring namePipe(L"\\\\");
	namePipe.append(server);
	namePipe.append(L"\\pipe\\");
	namePipe.append(pipe);
	std::wcout << L"Path to pipe: " << namePipe << std::endl;

	try
	{
		if ((cH = CreateFile(namePipe.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			NULL,
			NULL)) == INVALID_HANDLE_VALUE) {
			throw SetPipeError("createFile: ", GetLastError());
		}

		char wbuf[50]{"Hello from ClientNP"},
			 obuf[50]{};
		int messageCount;

		std::cout << "Сколько сообщений отправить? ";
		std::cin >> messageCount;

		for (int i = 0; i < messageCount; i++) {
			std::string message = wbuf + std::to_string(i);

			if (!WriteFile(cH, message.c_str(), message.length(), 0, NULL)) {
				throw SetPipeError("writeFile: ", GetLastError());
			}

			if (!ReadFile(cH, obuf, sizeof(obuf), 0, NULL)) {
				throw SetPipeError("readFile: ", GetLastError());
			}

			int k = strlen(obuf) - 1;
			std::string digit = "";

			while (isdigit(obuf[k]))
			{
				digit.insert(0, std::to_string(obuf[k--] - 48));
			}

			message = wbuf + std::to_string(atoi(digit.c_str()) + 1);

			if (!WriteFile(cH, message.c_str(), message.length(), 0, NULL)) {
				throw SetPipeError("writeFile: ", GetLastError());
			}

			if (!ReadFile(cH, obuf, sizeof(obuf), 0, NULL)) {
				throw SetPipeError("readFile: ", GetLastError());
			}
		}

		if (!WriteFile(cH, "", 1, 0, NULL)) {
			throw SetPipeError("writeFile: ", GetLastError());
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


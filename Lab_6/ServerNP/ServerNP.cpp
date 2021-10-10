#include <iostream>
#include <windows.h>
#include <string>
#include "../PipeError/framework.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "debug/PipeError.lib")

#define BASE_PATH_PIPE L"\\\\.\\pipe\\" 

int main()
{
	system("chcp 1251>nul");

	HANDLE sH;
	
	std::wstring namePipe(BASE_PATH_PIPE);
	std::wstring tmp;

	std::wcout << "Write name of pipe: ";
	std::wcin >> tmp;
	namePipe.append(tmp);

	try
	{
		while (true)
		{
			if ((sH = CreateNamedPipe(namePipe.c_str(), 
				PIPE_ACCESS_DUPLEX,
				PIPE_TYPE_MESSAGE | PIPE_WAIT,
				PIPE_UNLIMITED_INSTANCES,
				NULL,
				NULL,
				INFINITE,
				NULL)) == INVALID_HANDLE_VALUE) {
				throw SetPipeError("create: ", GetLastError());
			}

			std::cout << "Server is waiting for client..." << std::endl;

			if (!ConnectNamedPipe(sH, NULL))
				throw SetPipeError("create: ", GetLastError());

			char rbuf[50]{};

			while (true)
			{
				if (!ReadFile(sH, &rbuf, sizeof(rbuf), 0, NULL)) {
					throw SetPipeError("readFile: ", GetLastError());
				}

				if (strlen(rbuf) == 0) {
					DisconnectNamedPipe(sH);
					CloseHandle(sH);
					break;
				}

				std::cout << "Message from Client: " << rbuf << std::endl;

				if (!WriteFile(sH, &rbuf, sizeof(rbuf), 0, NULL)) {
					throw SetPipeError("writeFile: ", GetLastError());
				}
			}

			char answer;
			std::cout << "Завершить работу сервера? [Y(да)/N(нет)]? ";
			std::cin >> answer;

			if (answer == tolower('y')) {
				std::cout << "Завершение работы сервера...";
				break;
			}
		}
	}
	catch (std::string errorMsgText)
	{
		std::cout << errorMsgText << std::endl;
	}

	system("pause");
	return 0;
}


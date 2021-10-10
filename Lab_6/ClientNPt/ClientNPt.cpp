#include <iostream>
#include <windows.h>
#include <string>
#include "../PipeError/framework.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "../ServerNP/debug/PipeError.lib")

int main()
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

        char wbuf[50]{ "Hello from ClientNPt" },    
             obuf[50]{};

        DWORD bytes;
        DWORD state = PIPE_READMODE_MESSAGE;
        int messageCount;
        SetNamedPipeHandleState(cH, &state, NULL, NULL);

        std::cout << "Сколько сообщений отправить? ";
        std::cin >> messageCount;

        for (int i = 0; i < messageCount; i++) {
            if (TransactNamedPipe(cH, wbuf, sizeof(wbuf), obuf, sizeof(obuf), &bytes, NULL)) {
                std::cout << "Message from Server: " << obuf << std::endl;
            }
            else {
                throw SetPipeError("transactNamedPipe: ", GetLastError());
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

#include <iostream>
#include <windows.h>
#include <string>
#include "../PipeError/framework.h"

#pragma warning(disable : 4996)
#pragma comment(lib, "../ServerNP/debug/PipeError.lib")

int main()
{
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
        char wbuf[50]{ "Hello from ClientNPct" },
            obuf[50]{};

        DWORD bytes;

        if (CallNamedPipe(namePipe.c_str(), wbuf, sizeof(wbuf), obuf, sizeof(obuf), &bytes, 2)) {
            std::cout << "Message from Server: " << obuf << std::endl;
        }
        else {
            throw SetPipeError("createFile: ", GetLastError());
        }

        std::cout << "Client's work has been done.." << std::endl;
    }
    catch (std::string errorMsgText)
    {
        std::cout << errorMsgText << std::endl;
    }

    system("pause");
    return 0;
}

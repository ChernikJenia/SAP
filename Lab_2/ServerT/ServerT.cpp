#include <iostream>
#include "Winsock2.h"
#include <string>
#include <tchar.h>
#include "../ErrorMessageLib/framework.h"

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "../debug/ErrorMessageLib.lib")
#pragma warning(disable : 4996)

using namespace std;

int main(int argc, _TCHAR* argv[])
{
    system("chcp 1251>nul");

    WSADATA wsaData;
    SOCKET sS;
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("Socket: ", WSAGetLastError());

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = INADDR_ANY;

        if(bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMsgText("bind: ", WSAGetLastError());

        if(listen(sS, SOMAXCONN) == SOCKET_ERROR)
            throw SetErrorMsgText("listen: ", WSAGetLastError());

        SOCKET cS;
        SOCKADDR_IN clnt;
        memset(&clnt, 0, sizeof(clnt));
        int lclnt = sizeof(clnt);

        while (true)
        {
            if ((cS = accept(sS, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET)
                throw SetErrorMsgText("accept: ", WSAGetLastError());

            cout << inet_ntoa(clnt.sin_addr);
            cout << ": " << htons(clnt.sin_port);
            cout << " connected.." << endl;


            char ibuf[50]{};
            int libuf = 0,
                lobuf = 0;

            do
            {
                memset(ibuf, 0, sizeof(ibuf));

                if ((libuf = recv(cS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
                    throw SetErrorMsgText("recv: ", WSAGetLastError());

                cout << ibuf << endl;

                if ((lobuf = send(cS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
                    throw SetErrorMsgText("send: ", WSAGetLastError());

            } while (strlen(ibuf) > 0);

            if (closesocket(cS) == SOCKET_ERROR)
                throw SetErrorMsgText("CloseSocket: ", WSAGetLastError());

            char answer;
            cout << "Завершить работу сервера? [Y(да)/N(нет)]? ";
            cin >> answer;

            if (answer == tolower('y'))
                break;
        }

        if(closesocket(sS) == SOCKET_ERROR)
            throw SetErrorMsgText("CloseSocket: ", WSAGetLastError());

        if (WSACleanup() != 0)
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
    }
    catch (string errorMsgText) 
    {
        cout << endl << "WSAGetLastError: " << errorMsgText;
    }

    return 0;
}


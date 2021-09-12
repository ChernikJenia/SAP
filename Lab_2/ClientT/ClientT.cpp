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
    SOCKET cS;
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        if ((cS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("Socket: ", WSAGetLastError());

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (connect(cS, (sockaddr*)&serv, sizeof(serv)) == INVALID_SOCKET)
            throw SetErrorMsgText("connect: ", WSAGetLastError());

        char ibuf[50]{},
            obuf[50]{};
        int libuf = 0,
            lobuf = 0,
            msgCount = 0;

        cout << "Сколько сообщений отправить? ";
        cin >> msgCount;

        for (int i = 0; i < msgCount; i++) 
        {
            string msg = "Hello from Client " + to_string(i);

            if ((lobuf = send(cS,  msg.c_str(), msg.length(), NULL)) == SOCKET_ERROR)
                throw SetErrorMsgText("send: ", WSAGetLastError());

            if ((libuf = recv(cS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
                throw SetErrorMsgText("recv: ", WSAGetLastError());

           int k = strlen(ibuf) - 1;
           string digit = "";

           while (isdigit(ibuf[k]))
           {
               digit.insert(0, to_string(ibuf[k--] - 48)); // перевод из ascii
           }

           msg = "Hello from Client " + to_string(atoi(digit.c_str()) + 1);

           if ((lobuf = send(cS, msg.c_str(), msg.length(), NULL)) == SOCKET_ERROR)
               throw SetErrorMsgText("send: ", WSAGetLastError());

            if ((libuf = recv(cS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
                throw SetErrorMsgText("recv: ", WSAGetLastError());

        }
        
        if ((lobuf = send(cS, "", 1, NULL)) == SOCKET_ERROR)
            throw SetErrorMsgText("send: ", WSAGetLastError());
        
        if (closesocket(cS) == SOCKET_ERROR)
            throw SetErrorMsgText("CloseSocket: ", WSAGetLastError());

        if (WSACleanup() != 0)
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());


    }
    catch (string errorMsgText)
    {
        cout << endl << errorMsgText;
    }

	return 0;
}



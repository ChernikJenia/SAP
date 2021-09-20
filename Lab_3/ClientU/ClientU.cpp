#include <iostream>
#include "Winsock2.h"
#include <string>
#include "../../Lab_2/ErrorMessageLib/framework.h"

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "../../Lab_2/debug/ErrorMessageLib.lib")
#pragma warning(disable : 4996)

using namespace std;

int main(int argc, char* argv[])
{
    system("chcp 1251>nul");

    WSADATA wsaData;
    SOCKET cS;

    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        if ((cS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("Socket: ", WSAGetLastError());

        const char* serverAddress = "127.0.0.1";

        if (argc > 1)
        {
            serverAddress = argv[1];
        }

        SOCKADDR_IN to;
        to.sin_family = AF_INET;
        to.sin_port = htons(2000);
        to.sin_addr.s_addr = inet_addr(serverAddress);
        char ibuf[50]{};
        int lb = 0,
            msgCount = 0;

        cout << "Сколько сообщений отправить? ";
        cin >> msgCount;

        for (int i = 0; i < msgCount; i++)
        {
            string msg = "Hello from ClientU " + to_string(i);

            if ((lb = sendto(cS, msg.c_str(), msg.length() + 1, NULL, (sockaddr*)&to, sizeof(to))) == SOCKET_ERROR)
                throw SetErrorMsgText("sendto: ", WSAGetLastError());


            if (lb = recvfrom(cS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&to, &lb) == SOCKET_ERROR)
                throw SetErrorMsgText("recvfrom:, ", WSAGetLastError());


            int k = strlen(ibuf) - 1;
            string digit = "";

            while (isdigit(ibuf[k]))
            {
                digit.insert(0, to_string(ibuf[k--] - 48)); // перевод из ascii
            }

            msg = "Hello from ClientU " + to_string(atoi(digit.c_str()) + 1);

            if ((lb = sendto(cS, msg.c_str(), msg.length() + 1, NULL, (sockaddr*)&to, sizeof(to))) == SOCKET_ERROR)
                throw SetErrorMsgText("send: ", WSAGetLastError());

            if ((lb = recvfrom(cS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&to, &lb)) == SOCKET_ERROR)
                throw SetErrorMsgText("recv: ", WSAGetLastError());
        }

        if ((lb = sendto(cS, "", 2, NULL, (sockaddr*)&to, sizeof(to))) == SOCKET_ERROR)
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
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
    SOCKET sS;

    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("Socket: ", WSAGetLastError());

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = INADDR_ANY;

        if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMsgText("bind: ", WSAGetLastError());


        SOCKADDR_IN from;
        int lc = sizeof(from);
        char bfrom[50]{};
        int lb = 0;

        while (true)
        {
            memset(&from, 0, sizeof(from));
            memset(&bfrom, 0, sizeof(bfrom));

            while(true)
            {
                if (lb = recvfrom(sS, bfrom, sizeof(bfrom), NULL, (sockaddr*)&from, &lc) == SOCKET_ERROR)
                    throw SetErrorMsgText("recvfrom: ", WSAGetLastError());

                if (strlen(bfrom) == 0)
                {
                    cout << inet_ntoa(from.sin_addr) << " закончил отправку сообщений...." << endl;
                    break;
                }

                cout << "Message from " << inet_ntoa(from.sin_addr) << " : ";
                cout << bfrom << endl;

                if ((lb = sendto(sS, bfrom, strlen(bfrom) + 1, NULL, (sockaddr*)&from, sizeof(from))) == SOCKET_ERROR)
                    throw SetErrorMsgText("sendto: ", WSAGetLastError());

            }
        }
      

        if (closesocket(sS) == SOCKET_ERROR)
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


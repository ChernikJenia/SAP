#include <iostream>
#include "Winsock2.h"
#include <string>
#include "../../../Lab_2/ErrorMessageLib/framework.h"

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "../../../Lab_2/debug/ErrorMessageLib.lib")
#pragma warning(disable : 4996)

#define PORT 2000
#define CALL (char*)"Hello"

using namespace std;

SOCKET cS;

bool GetServerByName(char* name, char* call, SOCKADDR_IN* from, int &flen)
{
    int lb = 0, i = 0;
    char bfrom[50]{};
    struct in_addr addr;
    hostent* server = gethostbyname(name);

    if (server == NULL) {
        return false;
    }


    while (server->h_addr_list[i] != 0) {
        addr.s_addr = *(u_long*)server->h_addr_list[i++];
        printf("IP Address #%d: %s\n", i, inet_ntoa(addr));
    }
   

    from->sin_addr.s_addr = inet_addr(inet_ntoa(addr));


    if ((lb = sendto(cS, CALL, strlen(CALL) + 1, NULL, (sockaddr*)from, flen)) == SOCKET_ERROR)
        throw SetErrorMsgText("sendto: ", WSAGetLastError());

    cout << "Waiting for response...(maybe you need to restart CLientB)" << endl;

    if ((lb = recvfrom(cS, bfrom, sizeof(bfrom), NULL, (sockaddr*)from, &flen)) == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAETIMEDOUT)
            return false;

        throw SetErrorMsgText("GetRequestFromClient: ", WSAGetLastError());
    }

    return strcmp(CALL, bfrom) == 0;
}

int main(int argc, char* argv[])
{
    system("chcp 1251>nul");

    WSADATA wsaData;
    int optval = 1;
    
    
    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        if ((cS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("Socket: ", WSAGetLastError());

        SOCKADDR_IN from;
        int lc = sizeof(from);
        from.sin_family = AF_INET;
        from.sin_port = htons(PORT);

        if (argc < 2) {
            cout << "Error: specify a server name\n";
            system("pause");
            return 0;
        }

        if (!GetServerByName(argv[1], CALL, &from, lc)) {
            cout << "Can't find such a server\n";
        }

        if (closesocket(cS) == SOCKET_ERROR)
            throw SetErrorMsgText("CloseSocket: ", WSAGetLastError());

        if (WSACleanup() != 0)
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << endl << errorMsgText;
    }

    system("pause");
    return 0;
}



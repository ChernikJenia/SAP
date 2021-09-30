#include <iostream>
#include "Winsock2.h"
#include <string>
#include <vector>
#include "../../Lab_2/ErrorMessageLib/framework.h"

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "../../Lab_2/debug/ErrorMessageLib.lib")
#pragma warning(disable : 4996)

#define PORT 2000
#define CALL (char*)"Hello"

using namespace std;

SOCKET sS;

void SearchForServers(char* call, short port)
{
    SOCKET serverTest;
    int lb = 0,
        lc = 0, 
        optval = 1,
        timeout = 3000;
    vector<string> serversIP;

    if ((serverTest = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("Socket: ", WSAGetLastError());


    // По умолчанию использование стандартного широковещательного адреса не допускается и для его применения необходимо
    // использовать эту функцию

    if (setsockopt(serverTest, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
        throw SetErrorMsgText("opt: ", WSAGetLastError());

    if(setsockopt(serverTest, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int)) == SOCKET_ERROR)
        throw SetErrorMsgText("opt: ", WSAGetLastError());

    SOCKADDR_IN all;
    lc = sizeof(all);
    all.sin_family = AF_INET;
    all.sin_port = htons(PORT);
    all.sin_addr.s_addr = INADDR_BROADCAST;

     if ((lb = sendto(serverTest, CALL, strlen(CALL) + 1, NULL, (sockaddr*)&all, lc)) == SOCKET_ERROR)
            throw SetErrorMsgText("sendto: ", WSAGetLastError());
      
    while (true)
    {
        char bfrom[50]{};

        if ((lb = recvfrom(serverTest, bfrom, sizeof(bfrom), NULL, (sockaddr*)&all, &lc)) == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
            {
                if (serversIP.size() > 0) {
                    cout << "There are " << serversIP.size() << " available on " << port << " port: " << endl;

                    for (auto s : serversIP) {
                        cout << "\t" << s << endl;
                    }
                }
                else {
                    cout << "There are no more servers available on " << port << " port" << endl;
                }

                return;
            }

            throw SetErrorMsgText("GetRequestFromClient: ", WSAGetLastError());
        }

        if (strcmp(CALL, bfrom) == 0) {
            serversIP.push_back(inet_ntoa(all.sin_addr));
        }
    }
   
}


bool GetRequestFromClient(char* name, short port, struct sockaddr* from, int* flen)
{
    char bfrom[50]{};
    int lb = 0;

    while (true)
    {
        if ((lb = recvfrom(sS, bfrom, sizeof(bfrom), NULL, from, flen)) == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
            {
                return false;
            }

            throw SetErrorMsgText("GetRequestFromClient: ", WSAGetLastError());
        }
        else
        {
            if (strcmp(name, bfrom) == 0)
            {
                return true;
            }
        }
    }
}

bool PutAnswerToClient(char* name, struct sockaddr* to, int* lto)
{
    int lb = 0;

    if ((lb = sendto(sS, CALL, strlen(CALL) + 1, NULL, to, *lto)) == SOCKET_ERROR)
    {
        cout << SetErrorMsgText("sendto: ", WSAGetLastError()) << endl;
        return false;
    }
    return true;
}     


int main(int argc, char* argv[])
{
    system("chcp 1251>nul");

    WSADATA wsaData;
    SOCKADDR_IN serv;
    int optval = 1;
    int lc;

    try
    {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("Socket: ", WSAGetLastError());


        serv.sin_family = AF_INET;
        serv.sin_port = htons(PORT);
        serv.sin_addr.s_addr = INADDR_ANY;      

        if(bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMsgText("bind: ", WSAGetLastError());

       SearchForServers(CALL, PORT);

        cout << "Server is listening to " << PORT << " port..." << endl;

        SOCKADDR_IN from;
        lc = sizeof(from);

        memset(&from, 0, sizeof(from));
   
        while (true)
        {
            if (GetRequestFromClient(CALL, PORT, (sockaddr*)&from, &lc))
            {
                cout << "Client " << inet_ntoa(from.sin_addr) 
                    << ":" << ntohs(from.sin_port) 
                    << " sent the correct call sign" << endl;

                PutAnswerToClient(CALL, (sockaddr*)&from, &lc);
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

    system("pause");
    return 0;
}


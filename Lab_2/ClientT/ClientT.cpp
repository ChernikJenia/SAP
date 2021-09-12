#include <iostream>
#include "Winsock2.h"
#include <string>
#include <tchar.h>

#pragma comment(lib, "WS2_32.lib")
#pragma warning(disable : 4996)

using namespace std;

string SetErrorMsgText(int code)
{
    string msgText;

    switch (code)
    {
#pragma region switches
    case WSAEINTR:
        msgText = "WSAEINTR";
        break;
    case WSAEACCES:
        msgText = "WSAEACCES";
        break;
    case WSAEFAULT:
        msgText = "WSAEFAULT";
        break;
    case WSAEINVAL:
        msgText = "WSAEINVAL";
        break;
    case WSAEMFILE:
        msgText = "WSAEMFILE";
        break;
    case WSAEWOULDBLOCK:
        msgText = "WSAEWOULDBLOCK";
        break;
    case WSAEINPROGRESS:
        msgText = "WSAEINPROGRESS";
        break;
    case WSAEALREADY:
        msgText = "WSAEALREADY";
        break;
    case WSAENOTSOCK:
        msgText = "WSAENOTSOCK";
        break;
    case WSAEDESTADDRREQ:
        msgText = "WSAEDESTADDRREQ";
        break;
    case WSAEMSGSIZE:
        msgText = "WSAEMSGSIZE";
        break;
    case WSAEPROTOTYPE:
        msgText = "WSAEPROTOTYPE";
        break;
    case WSAENOPROTOOPT:
        msgText = "WSAENOPROTOOPT";
        break;
    case WSAEPROTONOSUPPORT:
        msgText = "WSAEPROTONOSUPPORT";
        break;
    case WSAESOCKTNOSUPPORT:
        msgText = "WSAESOCKTNOSUPPORT";
        break;
    case WSAEOPNOTSUPP:
        msgText = "WSAEOPNOTSUPP";
        break;
    case WSAEPFNOSUPPORT:
        msgText = "WSAEOPNOTSUPP";
        break;
    case WSAEAFNOSUPPORT:
        msgText = "WSAEOPNOTSUPP";
        break;
    case WSAEADDRINUSE:
        msgText = "WSAEADDRINUSE";
        break;
    case WSAEADDRNOTAVAIL:
        msgText = "WSAEADDRNOTAVAIL";
        break;
    case WSAENETDOWN:
        msgText = "WSAENETDOWN";
        break;
    case WSAENETUNREACH:
        msgText = "WSAENETUNREACH";
        break;
    case WSAECONNABORTED:
        msgText = "WSAECONNABORTED";
        break;
    case WSAECONNRESET:
        msgText = "WSAECONNRESET";
        break;
    case WSAENOBUFS:
        msgText = "WSAENOBUFS";
        break;
    case WSAEISCONN:
        msgText = "WSAEISCONN";
        break;
    case WSAENOTCONN:
        msgText = "WSAENOTCONN";
        break;
    case WSAESHUTDOWN:
        msgText = "WSAESHUTDOWN";
        break;
    case WSAETIMEDOUT:
        msgText = "WSAETIMEDOUT";
        break;
    case WSAECONNREFUSED:
        msgText = "WSAECONNREFUSED";
        break;
    case WSAEHOSTDOWN:
        msgText = "WSAEHOSTDOWN";
        break;
    case WSAEHOSTUNREACH:
        msgText = "WSAEHOSTUNREACH";
        break;
    case WSAEPROCLIM:
        msgText = "WSAEPROCLIM";
        break;
    case WSASYSNOTREADY:
        msgText = "WSASYSNOTREADY";
        break;
    case WSAVERNOTSUPPORTED:
        msgText = "WSAVERNOTSUPPORTED";
        break;
    case WSANOTINITIALISED:
        msgText = "WSANOTINITIALISED";
        break;
    case WSAEDISCON:
        msgText = "WSAEDISCON";
        break;
    case WSATYPE_NOT_FOUND:
        msgText = "WSATYPE_NOT_FOUND";
        break;
    case WSAHOST_NOT_FOUND:
        msgText = "WSAHOST_NOT_FOUND";
        break;
    case WSATRY_AGAIN:
        msgText = "WSATRY_AGAIN";
        break;
    case WSANO_RECOVERY:
        msgText = "WSANO_RECOVERY";
        break;
    case WSANO_DATA:
        msgText = "WSANO_DATA";
        break;
    case WSA_INVALID_HANDLE:
        msgText = "WSA_INVALID_HANDLE";
        break;
    case WSA_INVALID_PARAMETER:
        msgText = "WSA_INVALID_PARAMETER";
        break;
    case WSA_IO_INCOMPLETE:
        msgText = "WSA_IO_INCOMPLETE";
        break;
    case WSA_IO_PENDING:
        msgText = "WSA_IO_PENDING";
        break;
    case WSA_NOT_ENOUGH_MEMORY:
        msgText = "WSA_NOT_ENOUGH_MEMORY";
        break;
    case WSA_OPERATION_ABORTED:
        msgText = "WSA_OPERATION_ABORTED";
        break;
        /*    case WSAINVALIDPROCTABLE:
                msgText = "WSAINVALIDPROCTABLE";
                break;
            case WSAINVALIDPROVIDER:
                msgText = "WSAINVALIDPROVIDER";
                break;
            case WSAPROVIDERFAILEDINIT:
                msgText = "WSAPROVIDERFAILEDINIT";
                break;*/
    case WSASYSCALLFAILURE:
        msgText = "WSASYSCALLFAILURE";
        break;
    default:
        msgText = "unknown error";
        break;
    }
    #pragma endregion
    return msgText;
}

string SetErrorMsgText(string msgText, int code)
{
    return msgText + SetErrorMsgText(code);
}

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



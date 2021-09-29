#include <iostream>
#include "Winsock2.h"
#include <string>
#include "../../Lab_2/ErrorMessageLib/framework.h"

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "../../Lab_2/debug/ErrorMessageLib.lib")
#pragma warning(disable : 4996)

#define PORT 2000
#define CALL (char*)"Hello"


using namespace std;

SOCKET cS;

bool GetServer(char* call, short port, struct sockaddr* from, int* flen)
{
	int lb = 0;
	char bfrom[50]{};


	if ((lb = sendto(cS, CALL, strlen(CALL) + 1, NULL, from, *flen)) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto: ", WSAGetLastError());


	cout << "Waiting for response...(maybe you need to restart CLientB)" << endl;

	if ((lb = recvfrom(cS, bfrom, sizeof(bfrom), NULL, from, flen)) == SOCKET_ERROR)
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


		// По умолчанию использование стандартного широковещательного адреса не допускается и для его применения необходимо
		// использовать эту функцию
		if (setsockopt(cS, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
			throw SetErrorMsgText("opt: ", WSAGetLastError());


		SOCKADDR_IN all; 
		int lc = sizeof(all);
		all.sin_family = AF_INET; 
		all.sin_port = htons(PORT); 
		all.sin_addr.s_addr = INADDR_BROADCAST; 
		

		if (GetServer(CALL, PORT, (sockaddr*)&all, &lc))
		{
			cout << "server " << inet_ntoa(all.sin_addr) << " responded" << endl;
		}
		else
		{
			cout << "response isn't correct" << endl;
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
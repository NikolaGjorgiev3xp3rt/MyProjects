#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <cctype> // for isdigit function
using namespace std;

bool isNumeric(const char* str) {
    while (*str) {
        if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

int main()
{
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 2);
    if (WSAStartup(DllVersion, &wsaData) != 0)
    {
        MessageBoxA(NULL, "Neuspesna inicijalizacija", "Error", MB_OK | MB_ICONERROR);
        exit(1);
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(53555);
    addr.sin_family = AF_INET;

    SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0)
    {
        MessageBoxA(NULL, "Neuspesna konekcija", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }
    cout << "Uspesna vospostavena konekcija so serverot!" << std::endl;

    char PorakaSK[1000];
    char PorakaKS[1000];

    while (true) {
        cout << "Vnesi poraka: ";
        cin.getline(PorakaKS, sizeof(PorakaKS));

        if (isNumeric(PorakaKS)) {
            cout << "Ve molime vnesete nova poraka" << endl;
            continue;
        }

        send(Connection, PorakaKS, strlen(PorakaKS) + 1, NULL); // +1 to include null-terminator

        if (strcmp(PorakaKS, "kraj") == 0) {
            recv(Connection, PorakaSK, sizeof(PorakaSK), NULL);
            cout << "PorakaSK: " << PorakaSK << endl;
            break;
        }

        recv(Connection, PorakaSK, sizeof(PorakaSK), NULL);
        cout << "PorakaSK: " << PorakaSK << endl;
    }

    closesocket(Connection);
    WSACleanup();
    return 0;
}

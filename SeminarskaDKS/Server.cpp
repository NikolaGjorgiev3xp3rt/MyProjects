#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <unordered_set>
#include <cstring>
using namespace std;

bool unikatniKarakteri(const char* str, int& nonUniqueCount) {
    unordered_set<char> charSet;
    int length = strlen(str);
    nonUniqueCount = 0;

    for (int i = 0; i < length; ++i) {
        if (charSet.find(str[i]) != charSet.end()) {
            nonUniqueCount++;
        }
        charSet.insert(str[i]);
    }

    return nonUniqueCount == 0;
}

int main()
{
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 2);
    if (WSAStartup(DllVersion, &wsaData) != 0)
    {
        MessageBoxA(NULL, "Neuspesna inicijalizacija", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(53555);
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, 4);

    SOCKET newConnection;
    newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
    if (newConnection == 0)
    {
        cout << "Nastana greska pri prifakjanje na konekcija obidete se povtorno." << endl;
    }
    else
    {
        cout << "Uspesno prifatena konekcija!" << endl;
        char PorakaKS[1000];
        char PorakaSK[1000];

        while (true) {
            recv(newConnection, PorakaKS, sizeof(PorakaKS), NULL);
            if (strcmp(PorakaKS, "kraj") == 0) {
                strcpy_s(PorakaSK, sizeof(PorakaSK), "Kraj na sesijata");
                send(newConnection, PorakaSK, strlen(PorakaSK) + 1, NULL); // +1 to include null-terminator
                cerr << "Kraj na sesija" << endl;
                closesocket(newConnection);
                break;
            }
            else {
                int nonUniqueCount;
                bool isUnique = unikatniKarakteri(PorakaKS, nonUniqueCount);

                if (isUnique) {
                    strcpy_s(PorakaSK, sizeof(PorakaSK), "Isprativte unikatna poraka");
                }
                else {
                    snprintf(PorakaSK, sizeof(PorakaSK), "Porakata koja ja isprativte ne e unikatna. Porakata ima %d karakteri koi ne se unikatni", nonUniqueCount);
                }

                cout << "Poraka od klient: " << PorakaKS << endl;
                send(newConnection, PorakaSK, strlen(PorakaSK) + 1, NULL); // +1 to include null-terminator
            }
        }
    }

    closesocket(sListen);
    WSACleanup();
    return 0;
}

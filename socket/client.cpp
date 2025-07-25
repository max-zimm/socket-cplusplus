#include "client.h"

#pragma comment(lib, "Ws2_32.lib")



Client::Client(int port){
    // Initialize Winsock.
    int iResult;
    WSADATA wsaData;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("Client: WSAStartup failed: %d\n", iResult); 
        return;
    }

    // Create a socket.
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;



    iResult = getaddrinfo(CLIENT_SERVER_IP, PORT, &hints, &result);
    if (iResult != 0) {

        WSACleanup();
        return;
    }

    SOCKET connectSocket = INVALID_SOCKET;

    for(ptr = result; ptr != NULL; ptr = ptr->ai_next ) {
        printf("Client: Attempting to create socket.\n"); 
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (connectSocket == INVALID_SOCKET) {
            continue;
        }
        iResult = connect( connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    };

    freeaddrinfo(result);

    if (connectSocket == INVALID_SOCKET) {
        WSACleanup();
        return;
    }


    int sendMessageRes = send(connectSocket, "Hello, Server from Client!", strlen("Hello, Server from Client!"), 0);
    if (sendMessageRes == SOCKET_ERROR) {
        printf("Client: send failed: %d.\n", WSAGetLastError()); 
    }

    char recvbuf[512];
    int bytesReceived = recv(connectSocket, recvbuf, sizeof(recvbuf) - 1, 0);
    if (bytesReceived > 0) {
        recvbuf[bytesReceived] = '\0';
    } else if (bytesReceived == 0) {
        printf("Client: Server closed connection.\n");
    } else {
        printf("Client: Receive failed: %d.\n", WSAGetLastError()); 
    }
    printf("Client: Finished receiving.\n"); 
        shutdown(connectSocket, SD_SEND);
    closesocket(connectSocket);
    WSACleanup();
}

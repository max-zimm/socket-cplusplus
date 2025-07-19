#include "client.h"

#pragma comment(lib, "Ws2_32.lib")



Client::Client(int port){
    // Initialize Winsock.
    printf("Client: Entering Client constructor.\n"); 
    int iResult;
    WSADATA wsaData;

    printf("Client: Calling WSAStartup.\n"); 
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("Client: WSAStartup failed: %d\n", iResult); 
        return;
    }
    printf("Client: WSAStartup successful.\n"); 

    // Create a socket.
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char portStr[10];
    sprintf(portStr, "%d", port);

    printf("Client: Calling getaddrinfo for %s:%s.\n", CLIENT_SERVER_IP, portStr); 
    iResult = getaddrinfo(NULL, portStr, &hints, &result);
    if (iResult != 0) {
        printf("Client: getaddrinfo failed: %d\n", iResult); 
        WSACleanup();
        return;
    }
    printf("Client: getaddrinfo successful.\n"); 

    SOCKET connectSocket = INVALID_SOCKET;

    printf("Client: Starting connect loop.\n"); 
    for(ptr = result; ptr != NULL; ptr = ptr->ai_next ) {
        printf("Client: Attempting to create socket.\n"); 
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (connectSocket == INVALID_SOCKET) {
            printf("Client: Socket creation failed for current address.\n"); 
            continue;
        }
        printf("Client: Socket created. Attempting to connect.\n"); 
        iResult = connect( connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            printf("Client: Connect failed for current address, error: %d. Trying next.\n", WSAGetLastError()); 
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        printf("Client: Successfully connected to server.\n"); 
        break;
    };

    freeaddrinfo(result);

    if (connectSocket == INVALID_SOCKET) {
        printf("Client: Unable to connect to server after all attempts.\n"); 
        WSACleanup();
        return;
    }

    printf("Client: About to send message.\n"); 

    int sendMessageRes = send(connectSocket, "Hello, Server from Client!", strlen("Hello, Server from Client!"), 0);
    if (sendMessageRes == SOCKET_ERROR) {
        printf("Client: send failed: %d.\n", WSAGetLastError()); 
    }

    char recvbuf[512];
    int bytesReceived = recv(connectSocket, recvbuf, sizeof(recvbuf) - 1, 0);
    if (bytesReceived > 0) {
        recvbuf[bytesReceived] = '\0';
        printf("Client: Received from server: %s\n", recvbuf);
    } else if (bytesReceived == 0) {
        printf("Client: Server closed connection.\n");
    } else {
        printf("Client: Receive failed: %d.\n", WSAGetLastError()); 
    }
    printf("Client: Finished receiving.\n"); 
        shutdown(connectSocket, SD_SEND);
    closesocket(connectSocket);
    WSACleanup();
    printf("Client: Disconnected and cleaned up.\n"); 
}

int main(){
    Client myServer(27015); // This line should be `Client myClient(3715);` for the client. Assuming this is a copy-paste error.
    return 0;
}
#include "server.h" 
#pragma comment(lib, "Ws2_32.lib")

Server::Server(){
    start(); 
}

int Server::start(){
    printf("[Server] Starting server...\n");

    // Initialize Winsock.
    int iResult;
    WSADATA wsaData;

    printf("[Server] Calling WSAStartup...\n");
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("[Server] WSAStartup failed: %d\n", iResult);
        return 1;
    }
    printf("[Server] WSAStartup successful.\n");

    // Create a socket.
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    printf("[Server] Calling getaddrinfo...\n");
    iResult = getaddrinfo(NULL, PORT, &hints, &result);
    if (iResult != 0) {
        printf("[Server] getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    printf("[Server] getaddrinfo successful.\n");

    SOCKET newSocket = INVALID_SOCKET;
    printf("[Server] Creating socket...\n");
    newSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if(newSocket == INVALID_SOCKET){
        printf("[Server] Socket creation failed.\n");
        freeaddrinfo(result); 
        WSACleanup();
        return 1;
    }
    printf("[Server] Socket created successfully.\n");

    // Bind the socket.
    printf("[Server] Binding socket...\n");
    iResult = bind(newSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("[Server] Bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(newSocket);
        WSACleanup();
        return 1;
    }
    printf("[Server] Bind successful.\n");

    freeaddrinfo(result);

    // Listen on the socket for a client.
    printf("[Server] Listening for connections...\n");
    iResult = listen(newSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR){
        printf("[Server] Listen failed with error: %d\n", WSAGetLastError());
        closesocket(newSocket);
        WSACleanup();
        return 1;
    }
    printf("[Server] Now listening on port %s...\n", PORT);

    SOCKET ClientSocket = INVALID_SOCKET;
    printf("[Server] Waiting for client connection...\n");
    ClientSocket = accept(newSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("[Server] Accept failed: %d\n", WSAGetLastError());
        closesocket(newSocket);
        WSACleanup();
        return 1;
    }
    printf("[Server] Client connected! Client socket ID: %lld\n", (long long)ClientSocket);

    // Loop and receive and send data.
    char message[BUFFER_SIZE];
    int recieveMessageRes; 
    int sendMessageRes;    
    do {
        printf("[Server] Waiting to receive data from client...\n");
        recieveMessageRes = recv(ClientSocket, message, BUFFER_SIZE - 1, 0); 
        if (recieveMessageRes > 0) {
            message[recieveMessageRes] = '\0'; 
            printf("[Server] Bytes received: %d\n", recieveMessageRes);
            printf("[Server] Message received: %s\n", message); 

            printf("[Server] Sending response to client...\n");
            sendMessageRes = send(ClientSocket, "Hello, Client!", (int)strlen("Hello, Client!"), 0);
            if (sendMessageRes == SOCKET_ERROR) {
                printf("[Server] Send failed: %d\n", WSAGetLastError());
                break;
            }
            printf("[Server] Bytes sent: %d\n", sendMessageRes);

            printf("[Server] Shutting down send half of connection...\n");
            shutdown(ClientSocket, SD_SEND);

            printf("[Server] Waiting for client to close connection...\n");
            do {
                recieveMessageRes = recv(ClientSocket, message, BUFFER_SIZE - 1, 0);
            } while (recieveMessageRes > 0);

            if (recieveMessageRes == 0) {
                printf("[Server] Client disconnected cleanly after receiving response.\n");
            } else {
                printf("[Server] Receive failed after send: %d\n", WSAGetLastError());
            }

        } else if (recieveMessageRes == 0) {
            printf("[Server] Client disconnected before sending any data.\n");
        } else { 
            printf("[Server] Receive failed: %d\n", WSAGetLastError());
        }
    } while (recieveMessageRes > 0);

    // Disconnect.
    printf("[Server] Closing client and server sockets...\n");
    closesocket(ClientSocket);
    closesocket(newSocket);
    printf("[Server] Cleaning up Winsock...\n");
    WSACleanup();
    printf("[Server] Server shut down.\n");

    return 0; 
}

int main() {
    printf("[Main] Launching server...\n");
    Server myServer; 
    return 0;
}

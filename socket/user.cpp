#include "user.h"

Messenger::Messenger() {}
User Messenger::CreateUser(){


    User newUser;

    // CREATE NEW USERS SOCKET

    int iResult;
    WSADATA wsaData;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("Client: WSAStartup failed: %d\n", iResult); 
        return newUser;
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
        return newUser;
    }

    SOCKET connectSocket = INVALID_SOCKET;

    // connect user to server

    for(ptr = result; ptr != NULL; ptr = ptr->ai_next ) {
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
        return newUser;
    }

    
    newUser.userSocket = connectSocket;
    newUser.user_id = userCount;
    userCount++;
    userList.insert(userList.end(), newUser);
    return newUser;
}

void Messenger::SendMessage(User sender, char* message){
     int sendMessageRes = send(sender.userSocket, message, strlen(message), 0);
    if (sendMessageRes == SOCKET_ERROR) {
        printf("Client: send failed: %d.\n", WSAGetLastError()); 
    }

    char recvbuf[BUFFER_SIZE];
    int bytesReceived = recv(sender.userSocket, recvbuf, sizeof(recvbuf) - 1, 0);
    if (bytesReceived > 0) {
        recvbuf[bytesReceived] = '\0';
    }
    shutdown(sender.userSocket, SD_SEND);

}

char* Messenger::GetMessage(){
   static char messageBuffer[BUFFER_SIZE];

   scanf("%s", messageBuffer);
   return messageBuffer;

   
}

int main(){
    Messenger clients;

    User newUser = clients.CreateUser();

    char* message = clients.GetMessage();
    clients.SendMessage(newUser, message);
    return 0;
}
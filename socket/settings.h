#define _WIN32_WINNT 0x0501
#define WINVER       0x0501
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdio.h>    
#include <string.h>  
#pragma comment(lib, "Ws2_32.lib")

#define BUFFER_SIZE 512
#define PORT "27015"
#define CLIENT_SERVER_IP "127.0.0.1"
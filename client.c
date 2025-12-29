/* client.c - Run this on the Friend's computer */
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") 

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char message[2000], server_reply[2000];
    char ip_address[50];

    printf("\nInitializing Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");

    // 1. Create socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        printf("Could not create socket: %d", WSAGetLastError());
    }

    // Ask for Server IP
    printf("Enter Server IP address: ");
    scanf("%s", ip_address);
    getchar(); // Consume the newline character left by scanf

    server.sin_addr.s_addr = inet_addr(ip_address);
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    // 2. Connect to remote server
    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0) {
        printf("Connection error! Check IP address and Firewall.\n");
        return 1;
    }
    printf("Connected! You can type now.\n");

    // 3. Chat Loop
    while(1) {
        printf("\nYou: ");
        fgets(message, 2000, stdin);

        // Send data
        if( send(s , message , strlen(message) , 0) < 0) {
            printf("Send failed");
            return 1;
        }

        // Receive reply
        memset(server_reply, 0, 2000); // Clear buffer
        
        printf("Waiting for reply...\n");
        if( recv(s , server_reply , 2000 , 0) < 0) {
            printf("Recv failed (Server disconnected).");
            break;
        }
        printf("Server: %s", server_reply);
    }

    closesocket(s);
    WSACleanup();
    return 0;
}

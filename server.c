/* server.c - Run this on the Host computer */
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") // Linker directive for Winsock

int main() {
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;
    char buffer[2000];
    char reply[2000];

    printf("\nInitializing Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");

    // 1. Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        printf("Could not create socket: %d", WSAGetLastError());
    }

    // 2. Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    server.sin_port = htons( 8888 );     // Port number

    // 3. Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return 1;
    }
    
    // 4. Listen
    listen(s , 3);
    printf("Waiting for incoming connections on port 8888...\n");

    // 5. Accept connection
    c = sizeof(struct sockaddr_in);
    new_socket = accept(s , (struct sockaddr *)&client, &c);
    
    if (new_socket == INVALID_SOCKET) {
        printf("Accept failed");
        return 1;
    }
    printf("Connection accepted! You can now start chatting.\n");

    // 6. Chat Loop
    while(1) {
        // Clear buffer
        memset(buffer, 0, 2000);
        
        // Receive a message from client
        int recv_size = recv(new_socket, buffer, 2000, 0);
        if(recv_size == SOCKET_ERROR || recv_size == 0) {
            printf("Client disconnected.\n");
            break;
        }
        
        printf("\nClient: %s", buffer);

        // Send a reply
        printf("\nYou: ");
        fgets(reply, 2000, stdin);
        
        send(new_socket, reply, strlen(reply), 0);
    }

    closesocket(s);
    WSACleanup();
    return 0;
}

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct sockaddr_in sockaddr_in;

void handleError(char* message, int status) {
    perror(message);
    exit(status);
}

void getIPAndPort (int argc, char* argv[], char server[], uint16_t* port) {
    *port = 8080;
    strcpy(server, "0.0.0.0");
    if(argc == 3) {
        *port = atoi(argv[2]);
        strcpy(server, argv[1]);
    }
    if(argc > 3) {
        handleError("Invalid arguments: server [host] [port]", 7);
    }
}

void nameInfo(char descriptiveMessage[], sockaddr_in* name) {
    char ip[INET_ADDRSTRLEN];

    struct in_addr ipAddr = name->sin_addr;
    inet_ntop(AF_INET, &ipAddr, ip, INET_ADDRSTRLEN);

    printf("%s -- %s:%hu\n", descriptiveMessage, ip, ntohs(name->sin_port));
}

int socketInit() {
    int descriptor = socket(AF_INET, SOCK_STREAM, 0);
    return descriptor;
}

int connectToServer(char ip[], uint16_t port) {
    int socketDescriptor = socketInit();
    if (socketDescriptor < 0){
        handleError("SOCKET_ERROR\n An error occurred while creating the socket", 1);
    }

    sockaddr_in serverName;
    socklen_t nameSize = sizeof(serverName);

    memset (&serverName, 0, sizeof (serverName));
    serverName.sin_port        = htons(port);
    serverName.sin_family      = AF_INET;
    serverName.sin_addr.s_addr = inet_addr(ip);

    int serverSocketDescriptor = connect(socketDescriptor, (struct sockaddr*) &serverName, nameSize);
    if(serverSocketDescriptor < 0) {
        handleError("CONNECT_ERROR\nError while connecting to the server", 3);
    }

    nameInfo("Server", (sockaddr_in*) &serverName);
    return serverSocketDescriptor;
}

int main(int argc, char* argv[]) {
    uint16_t port = 0;
    char ip[INET_ADDRSTRLEN];

    getIPAndPort(argc, argv, ip, &port);
    int serverSocketDescriptor = connectToServer(ip, port);
    close(serverSocketDescriptor);
}

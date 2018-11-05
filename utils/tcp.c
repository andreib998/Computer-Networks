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

int acceptConn(int serverSocketDescriptor) {
    struct sockaddr clientName;
    socklen_t clientNameSize = sizeof(clientName);

    // Create an active socket with the same information as the passive socket
    // in order to send and receive data.
    // Check out the README.md file!
    int clientSocketDescriptor = accept(
                                        serverSocketDescriptor,
                                        &clientName,
                                        &clientNameSize
                                        );
    if(clientSocketDescriptor < 0) {
      handleError("Cannot accept connection.", 3);
    }

    // We are costing the sockaddr structure to sockaddr_in structure in order to
    // get the sin_addr and the other fields, otherwise everything except the
    // sin_addr will be inside a char[14].
    nameInfo("Client", (sockaddr_in*) &clientName);
    return clientSocketDescriptor;
}

int assignNameToSocket(int socketDescriptor, int port) {
    sockaddr_in name;
    name.sin_port        = htons(port);
    name.sin_family      = AF_INET;
    name.sin_addr.s_addr = htons(INADDR_ANY);

    socklen_t nameSize = sizeof(name);
    int bindResult = bind(socketDescriptor, (const struct sockaddr*) &name, nameSize);
    return bindResult;
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

void handleError(char* message, int status) {
    perror(message);
    exit(status);
}

int getPort (int argc, char* argv[]) {
    int defaultPort = 8080;
    if(argc == 2) {
        defaultPort = atoi(argv[1]);
        return defaultPort;
    }
    if(argc > 2) {
        handleError("Invalid arguments: server [port]", 7);
    }
    return defaultPort;
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

int passiveSocket(int port) {
    // Create a passive socket to listen for connections.
    int socketDescriptor = socketInit();
    if (socketDescriptor < 0){
        handleError("SOCKET_ERROR\n An error occurred while creating the socket", 1);
    }
    int bindResult = assignNameToSocket(socketDescriptor, port);
    if (bindResult < 0){
        handleError("ASSIGN_NAME_TO_SOCKET\n An error occurred during the binding process. This program failed to assign a name to the created socket.", 2);
    }
    return socketDescriptor;
}

int socketInit() {
    int descriptor = socket(AF_INET, SOCK_STREAM, 0);
    return descriptor;
}

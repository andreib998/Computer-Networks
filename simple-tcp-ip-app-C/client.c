#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct sockaddr_in sockaddr_in;

void handleError(char* message, int status) {
    perror(message);
    exit(status);
}

int socketInit() {
    int descriptor = socket(AF_INET, SOCK_STREAM, 0);
    return descriptor;
}

int assignNameToSocket(int socketDescriptor) {
    sockaddr_in name;
    name.sin_port        = htons(8080);
    name.sin_family      = AF_INET;
    name.sin_addr.s_addr = htons(INADDR_ANY);

    socklen_t nameSize = sizeof(name);
    int bindResult = bind(socketDescriptor, (const struct sockaddr*) &name, nameSize);
    return bindResult;
}

int passiveSocket(){
    // Create a passive socket to listen for connections
    int socketDescriptor = socketInit();
    if (socketDescriptor < 0){
        handleError("SOCKET_ERROR\n An error occurred while creating the socket", 1);
    }

    int bindResult = assignNameToSocket(socketDescriptor);
    if (bindResult < 0){
        handleError("ASSIGN_NAME_TO_SOCKET\n An error occurred during the binding process. This program failed to assign a name to the created socket.", 2);
    }

    return socketDescriptor;
}

void nameInfo(char descriptiveMessage[], sockaddr_in* name) {
    char ip[INET_ADDRSTRLEN];

    struct in_addr ipAddr = name->sin_addr;
    inet_ntop(AF_INET, &ipAddr, ip, INET_ADDRSTRLEN);

    printf("%s -- %s:hu\n", descriptiveMessage, ip, ntohs(name->sin_port));
}

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

int main() {

    int thisSocketDescriptor = passiveSocket();
    listen(thisSocketDescriptor, 5);

    int clientSocketDescriptor;
    printf("Server waiting for data on port 8080: \n");
    while(1) {

        clientSocketDescriptor = acceptConn(thisSocketDescriptor);

        close(clientSocketDescriptor);
    }

    return 0;
}

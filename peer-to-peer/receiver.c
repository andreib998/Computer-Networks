#include "../utils/headers.c"

void handleError(char message[], int status) {
    perror(message);
    exit(status);
}

int main() {
    int socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketDescriptor < 0) {
        handleError("socket", 1);
    }

    int broadcast = 1;
    if (setsockopt(socketDescriptor, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof broadcast) == -1) {
        handleError("setsockopt", 2);
    }

    struct sockaddr_in serverName;
    serverName.sin_addr.s_addr = INADDR_ANY;
    serverName.sin_port = htons(8080);
    serverName.sin_family = AF_INET;

    int status = bind(socketDescriptor, (struct sockaddr*) &serverName, sizeof(serverName));
    if(status < 0) {
        handleError("bind", 3);
    }

    char message[1024];
    struct sockaddr_in destinationName;
    socklen_t destinationSize = sizeof(destinationName);
    int bytesReceived = recvfrom(socketDescriptor, message, sizeof(message), 0, (struct sockaddr*) &destinationName, &destinationSize);
    printf("bytes recevied: %d\n", bytesReceived);
    printf("received message: %s\n", message);

    char acc[] = "message received";
    int bytesSent = sendto(socketDescriptor, acc, sizeof(acc), 0, (struct sockaddr*) &destinationName, sizeof(destinationName));
    printf("bytes sent: %d\n", bytesSent);

    return 0;
}

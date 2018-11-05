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

    // I as a receiver I accept packages from any IP -- INADDR_ANY.
    struct sockaddr_in name;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = htons(0);
    name.sin_family = AF_INET;

    int status = bind(socketDescriptor, (struct sockaddr*) &name, sizeof(name));
    if(status < 0) {
        handleError("bind", 3);
    }

    struct sockaddr_in serverName;
    serverName.sin_addr.s_addr = inet_addr("192.168.100.255");
    serverName.sin_port = htons(8080);
    serverName.sin_family = AF_INET;

    char message[] = "Hi, it's me, the server!";
    int bytesSent = sendto(socketDescriptor, message, strlen(message), 0, (struct sockaddr*) &serverName, sizeof(serverName));
    printf("bytes sent: %d\n", bytesSent);

    return 0;
}

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

    struct sockaddr_in serverName;
    serverName.sin_addr.s_addr = inet_addr("192.168.100.8");
    serverName.sin_port = htons(8080);
    serverName.sin_family = AF_INET;

    char message[] = "Hi, it's me, the server!";
    int bytesSent = sendto(socketDescriptor, message, strlen(message), 0, (struct sockaddr*) &serverName, sizeof(serverName));
    printf("bytes sent: %d\n", bytesSent);

    return 0;
}

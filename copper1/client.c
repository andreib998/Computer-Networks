#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>

int main(int argc, char *argv[]) {
       int x = 8080;
       if (argc > 1) {
           x = atoi(argv[1]);
       }
       int c;
       struct sockaddr_in server;

       c = socket(AF_INET, SOCK_STREAM, 0);
       if (c < 0) {
              printf("Socker build failed.\n");
              return 1;
       }

       memset(&server, 0, sizeof(server));
       server.sin_port = htons(x);
       server.sin_family = AF_INET;
       server.sin_addr.s_addr = inet_addr("127.0.0.1");

       if (connect(c, (struct sockaddr *) &server, sizeof(server)) < 0) {
              printf("Connection error.\n");
              return 1;
       }

       char message[100];
       printf("message: ");
       fgets(message, 100, stdin);

       uint16_t message_size = strlen(message);
       uint16_t size = htons(message_size);
       send(c, &size, sizeof(size), 0);

       for(int i = 0; i < message_size; i++) {
           send(c, &message[i], sizeof(char), 0);
           printf("%c", message[i]);
       }

       // uint16_t spaces = 0;
       // recv(c, &spaces, sizeof(spaces), 0);
       // spaces = ntohs(spaces);
       //
       // printf("Number of spaces %hu\n", spaces);
       close(c);
}

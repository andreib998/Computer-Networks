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
       int server_fd;
       struct sockaddr_in server, client;
       int c, l;

       server_fd = socket(AF_INET, SOCK_STREAM, 0);
       if (server_fd < 0) {
              printf("Error during server build.\n");
              return 1;
       }

       memset(&server, 0, sizeof(server));
       server.sin_port = htons(x);
       server.sin_family = AF_INET;
       server.sin_addr.s_addr = INADDR_ANY;

       // annouce that there is a server listening on the specified port
       if (bind(server_fd, (struct sockaddr *) &server, sizeof(server)) < 0) {
              printf("Binding error.\n");
              return 1;
       }

       listen(server_fd, 5);

       l = sizeof(client);
       memset(&client, 0, sizeof(client));

       while (1) {

              c = accept(server_fd, (struct sockaddr *) &client, &l);
              printf("Connected client.\n");

              uint16_t message_size;
              recv(c, &message_size, sizeof(message_size), MSG_WAITALL);
              message_size = ntohs(message_size);

              printf("Message size: %d\n", message_size);
              uint16_t spaces = 0;

              char character;
              for(int i = 0; i < message_size; i++) {
                  recv(c, &character, sizeof(char), MSG_WAITALL);
                  printf("%c", character);
                  if (character == ' ') {
                      spaces += 1;
                  }
              }

              // send(c, &spaces, sizeof(spaces), 0);
              // spaces = ntohs(spaces);

              close(c);
              // sfarsitul deservirii clientului;
       }
}

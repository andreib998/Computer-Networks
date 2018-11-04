When accepting an incoming request:
```
    int clientSocketDescriptor = accept(
                                        serverSocketDescriptor,
                                        (struct sockaddr*) &clientName,
                                        &clientNameSize
                                        );
```
Here is the trick: Since the accept function requires a struct sockaddr pointer, we can cast our struct sockaddr_in having sa_family_t type size as first argument in both structures.
```
int accept(int socket, struct sockaddr* restrict address, socklen_t *restrict address_len);

struct sockaddr_in {
         sa_family_t    sin_family; /* address family: AF_INET */
         in_port_t      sin_port;   /* port in network byte order */
         struct in_addr sin_addr;   /* internet address */
};

struct sockaddr {
   sa_family_t sa_family;
   char        sa_data[14];
}

/* Internet address. */
struct in_addr {
   uint32_t       s_addr;     /* address in network byte order */
};
```

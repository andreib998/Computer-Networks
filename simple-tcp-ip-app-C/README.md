When accepting an incoming request:
```c
int clientSocketDescriptor = accept(
                                    serverSocketDescriptor,
                                    (struct sockaddr*) &clientName,
                                    &clientNameSize
                                    );
```
Here is the trick: Since the accept function requires a struct sockaddr pointer, we can cast our struct sockaddr_in.
```c
int accept(int socket, struct sockaddr* restrict address, socklen_t *restrict address_len);

struct sockaddr_in {
    short           sin_family;       /* 2 bytes */
    unsigned short  sin_port;         /* 2 bytes */
    struct in_addr  sin_addr;         /* 4 bytes */
    char            sin_zero[8];      /* 8 bytes */
};

struct sockaddr {
   sa_family_t sa_family;             /* 2 bytes */
   char        sa_data[14];           /* 14 bytes */
}

/* Internet address. */
struct in_addr {
   uint32_t       s_addr;     /* address in network byte order */
};
```

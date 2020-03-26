#include <stdio.h>              // printf, frintf
#include <stdlib.h>             // exit, free, malloc
#include <string.h>             // strigns, strerror()
#include <assert.h>             // assert in compute_icmp_checksum
#include <errno.h>              // errno
#include <unistd.h>             // getpid()
#include <netinet/in.h>         //struct sockaddr_in 
#include <netinet/ip_icmp.h>    // struct icmp, IP_MAXPACKET
#include <sys/time.h>           // struct timezone, timersub, gettimeofday()
#include <sys/types.h>          // according to earlier standards
#include <sys/select.h>         // select()
#include <sys/socket.h>         // socket()
#include <arpa/inet.h>          // inet_ntop/pton()


struct data{    // struct used to get informations from received packets 
    char *ip;
    uint8_t type, code;
    uint16_t id, seq;
};

// Functions in check.c - data and functions checked for correctness
u_int16_t compute_icmp_checksum (const void *buff, int length);
const char *Inet_ntop(int af, const void *src, char *dst, socklen_t size);
int Inet_pton(int af, const char *src, void *dst);

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

int Socket(int domain, int type, int protocol);
int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);

void error_handle(const char *str, char *err);

int Gettimeofday(struct timeval *tv, struct timezone *tz);

int check_enter(int num, char *table[]);

// Function in network.c - similar to traceroute -I
void traceroute(char *ip);
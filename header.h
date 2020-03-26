#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <sys/types.h>          /* See NOTES - not required but in use in historical implementations*/
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//Functions in check.c - data and functions checked for corresctness
u_int16_t compute_icmp_checksum (const void *buff, int length); //function from lecture
const char *Inet_ntop(int af, const void *src, char *dst, socklen_t size);
int Inet_pton(int af, const char *src, void *dst);

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

int Socket(int domain, int type, int protocol);
int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);

void error_handle(const char *str, char *err);

int Gettimeofday(struct timeval *tv, struct timezone *tz);
void check_enter(int num, char *table[]);

//Functions in network.c - network handle
void traceroute(char *ip);
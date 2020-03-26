#include "header.h"

// cmain function of check.c is define functions which handle errors 

void error_handle(const char *str, char *err){
    fprintf(stderr, str, err);
    exit(EXIT_FAILURE); 
}


u_int16_t compute_icmp_checksum (const void *buff, int length){ // lecture function
    u_int32_t sum;
    const u_int16_t* ptr = buff;
    assert (length % 2 == 0);
    for (sum = 0; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16) + (sum & 0xffff);
    return (u_int16_t)(~(sum + (sum >> 16)));
}


const char *Inet_ntop(int af, const void *src, char *dst, socklen_t size){
    const char* rv = inet_ntop(af, src, dst, size);
    if(rv == NULL)
        error_handle("Inet_ntop error: %s\n", strerror(errno));
    return rv;
}


int Inet_pton(int af, const char *src, void *dst){
    int rv = inet_pton(af, src, dst);
    if(rv < 0)
        error_handle("Inet_pton error: %s\n", strerror(errno));
    return rv;
}


int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout){
    int rv = select(nfds, readfds, writefds, exceptfds, timeout);
    if(rv < 0)
        error_handle("Select error: %s\n", strerror(errno));
    return rv;
}


int Socket(int domain, int type, int protocol){
    int rv = socket(domain, type, protocol);
    if (rv < 0)
        error_handle("Socket error %s\n", strerror(errno));
    return rv;
}


int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen){
    int rv = setsockopt(sockfd, level, optname, optval, optlen);
    if (rv < 0)
        error_handle("Setsockopt error %s\n", strerror(errno));
    return rv;
}


ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen){
    ssize_t rv = recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
    if (rv < 0)
        error_handle("Recvfrom error %s\n", strerror(errno));
    return rv;
}


ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen){
    ssize_t rv = sendto(sockfd, buf, len, flags, dest_addr, addrlen);
    if (rv < 0)
        error_handle("Sendto error %s\n", strerror(errno));
    return rv;
}


int Gettimeofday(struct timeval *tv, struct timezone *tz){
    int rv = gettimeofday(tv, tz);
    if(rv < 0)
        error_handle("Gettimeofday error %s\n", strerror(errno));
    return rv;
}


int check_enter(int arg_num, char *table[]){
    if(arg_num != 2)
        error_handle("Number of arguments incorrect\n", NULL);
    
    struct sockaddr_in recipient;
    return Inet_pton(AF_INET, table[1], &recipient.sin_addr);

    /* Sometimes it is better to search a little bit before implementing your own functions
    char *ip = table[1];
    int number_count = 0, byte = 0, byte_count = 0;
    
    while(*ip != '\0'){
        
        if(*ip >= '0' && *ip <= '9'){
            if(number_count == 0)
                byte_count++;
            number_count++;
        }
        
        else{
            if(number_count == 0 || *ip != '.') //invalid symbol or to many dots in ip address
                error_handle("Invalid address %s\n", table[1]);
            byte = 0;
            number_count = 0;
        }

        byte *= 10;
        byte += (int) (*ip - '0');
        
        if(byte > 255) //too high number in ip address
            error_handle("Invalid address %s\n", table[1]);

        ip++;
    }

    if(byte_count != 4) //incorrect number of bytes in ip address
        error_handle("Invalid address %s\n", table[1]);
    */
}
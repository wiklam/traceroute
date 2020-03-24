#include "header.h"

u_int16_t compute_icmp_checksum (const void *buff, int length){ //function from lecture
    u_int32_t sum;
    const u_int16_t* ptr = buff;
    assert (length % 2 == 0);
    for (sum = 0; length > 0; length -= 2)
        sum += *ptr++;
    sum = (sum >> 16) + (sum & 0xffff);
    return (u_int16_t)(~(sum + (sum >> 16)));
}


int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout){
    int rv = select(nfds, readfds, writefds, exceptfds, timeout);
    if(rv < 0){
        fprintf(stderr, "Select error\n");
        exit(EXIT_FAILURE);
    }
    return rv;
}


int Socket(int domain, int type, int protocol){
    int rv = socket(domain, type, protocol);
    if (rv < 0){
        fprintf(stderr, "Socket error\n");
        exit(EXIT_FAILURE);
    }
    return rv;
}


ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen){
    ssize_t rv = recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
    if (rv < 0){
        fprintf(stderr, "Recvfrom error\n");
        exit(EXIT_FAILURE);
    }
    return rv;
}


ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen){
    ssize_t rv = sendto(sockfd, buf, len, flags, dest_addr, addrlen);
    if (rv < 0){
        fprintf(stderr, "Sendto error\n");
        exit(EXIT_FAILURE);
    }
    return rv;
}


void check_enter(int arg_num, char *table[]){
    if(arg_num != 2){
        fprintf(stderr, "Number of arguments incorrect\n");
        exit(EXIT_FAILURE);
    }
    
    char *ip = table[1];
    int number_count = 0, byte = 0, byte_count = 0;
    
    while(*ip != '\0'){
        
        if(*ip >= '0' && *ip <= '9'){
            if(number_count == 0)
                byte_count++;

            number_count++;
        }
        
        else{
            if(number_count == 0 || *ip != '.'){ //invalid symbol or to many dots in ip address
                fprintf(stderr, "Invalid address\n");
                exit(EXIT_FAILURE);
            }
            
            byte = 0;
            number_count = 0;
        }

        byte *= 10;
        byte += (int) (*ip - '0');
        
        if(byte > 255){ //too high number in ip address
            fprintf(stderr, "Invalid address\n");
            exit(EXIT_FAILURE);
        }

        ip++;
    }

    if(byte_count != 4){ //incorrect number of bytes in ip address
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    }
}
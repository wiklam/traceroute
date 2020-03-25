#include "header.h"

void set_icmp_header(struct icmp *header, int seq){
    header->icmp_type = ICMP_ECHO;
    header->icmp_code = 0;
    header->icmp_hun.ih_idseq.icd_id = getpid();
    header->icmp_hun.ih_idseq.icd_seq = 1;
    header->icmp_cksum = 0;
    header->icmp_cksum = compute_icmp_checksum((u_int16_t*)header, sizeof(*header));
}

void set_sockaddr_in(struct sockaddr_in *recipient, char *ip){
    bzero(recipient, sizeof(*recipient));
    recipient->sin_family = AF_INET;
    Inet_pton(AF_INET, ip, &recipient->sin_addr);
}

void sendpackets(int fd1, int ttl, char *ip, struct sockaddr_in *recipient){
    for(int it = 0; it < 3; it++){
        struct icmp header;
        set_icmp_header(&header, ttl * 3 + it);  //unique seq number for each packet in process

        Setsockopt(fd1, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));

        Sendto(fd1, &header, sizeof(header), 0, (struct sockaddr*) recipient, sizeof(*recipient));
    }
}

void recevepackets(int fd1){
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    u_int8_t buffer [IP_MAXPACKET];

    ssize_t packet_len = Recvfrom(fd1, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len);

    char sender_ip_str[20];
    Inet_ntop(AF_INET, &(sender.sin_addr),sender_ip_str, sizeof(sender_ip_str));
    
    struct ip* ip_header = (struct ip*) buffer;
    u_int8_t* icmp_packet = buffer + 4 * ip_header->ip_hl;
    struct icmp* icmp_header = (struct icmp*) icmp_packet;
}

void traceroute(char *ip){
    int sockfd1 = Socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    struct sockaddr_in recipient;
    set_sockaddr_in(&recipient, ip);

    int ttl = 1;
    int arrive = 0;

    while(ttl <= 30 && !arrive){
        sendpackets(sockfd1, ttl, ip, &recipient);
        recevepackets(sockfd1);
        ttl++;
    }
}
#include "header.h"


static void set_icmp_header(struct icmp *header, int seq){
    header->icmp_type = ICMP_ECHO;
    header->icmp_code = 0;
    header->icmp_hun.ih_idseq.icd_id = getpid();    // allows to distinguish packets from different process
    header->icmp_hun.ih_idseq.icd_seq = seq;        // unique number for packet in our process
    header->icmp_cksum = 0;
    header->icmp_cksum = compute_icmp_checksum((u_int16_t*) header, sizeof(*header));
}


static void set_sockaddr_in(struct sockaddr_in *recipient, char *ip){
    bzero(recipient, sizeof(*recipient));
    recipient->sin_family = AF_INET;
    Inet_pton(AF_INET, ip, &recipient->sin_addr);
}


static int get_data(u_int8_t *buffer, struct sockaddr_in *sender, struct data *data){
    data->ip = malloc(20 * sizeof(char));
    Inet_ntop(AF_INET, &(sender->sin_addr), data->ip, 20);
    
    struct ip* ip_header = (struct ip*) buffer;
    u_int8_t* icmp_packet = buffer + 4 * ip_header->ip_hl;
    struct icmp* icmp_header = (struct icmp*) icmp_packet;

    data->type = icmp_header->icmp_type;
    data->code = icmp_header->icmp_code;

    if(data->type == ICMP_ECHOREPLY && data->code == 0){    // Echo reply
        data->id = icmp_header->icmp_id;
        data->seq = icmp_header->icmp_seq;
        return 1;
    }
    if(data->type == ICMP_TIME_EXCEEDED && data->code == 0){    // Time exceed message with code time to live exceed in transit
        ip_header = (struct ip*) &icmp_header->icmp_data;
        struct icmp* new_icmp_header = (void*) ip_header + 4 * ip_header->ip_hl;
        data->id = new_icmp_header->icmp_id;
        data->seq = new_icmp_header->icmp_seq;
        return 1;
    }
    return 0;
}


static void output(int ttl, int reached, char *ip[3], struct timeval times[3]){
    printf("%d. ", ttl);
    if(reached == 0){
        printf("* * *\n");
        return;
    }
    for(int it = 0; it < reached; it++){    // if there is an ip which wasn't printed, print it
        int diff = 1;
        for(int iter = it -1; iter >= 0; iter--){
            if(strcmp(ip[iter], ip[it]) == 0)
                diff = 0;
        }
        if(diff)
            printf("%s ", ip[it]);
    }
    if(reached == 3){   // if we catched all response packets calculate the average time
        float sum = (times[0].tv_sec + times[1].tv_sec + times[2].tv_sec) * 1000 + times[0].tv_usec + times[1].tv_usec + times[2].tv_usec;
        printf("%.3f ms\n", sum/3000);
    }
    else
        printf("???\n");
}


static int get_one_packet(int sfd, struct data *data){
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    u_int8_t buffer[IP_MAXPACKET];

    Recvfrom(sfd, buffer, IP_MAXPACKET, 0, (struct sockaddr*) &sender, &sender_len);

    return get_data(buffer, &sender, data);
}


static void sendpackets(int fd1, int ttl, struct sockaddr_in *recipient){
    for(int it = 0; it < 3; it++){
        struct icmp header;
        set_icmp_header(&header, ttl * 3 + it);  // unique seq number for each packet in process - ttl * 3 + it

        Setsockopt(fd1, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));

        Sendto(fd1, &header, sizeof(header), 0, (struct sockaddr*) recipient, sizeof(*recipient));
    }
}


static int receivepackets(int sfd, int ttl){
    int arrived = 0;
    int reached = 0;

    char *ip_address[3];

    struct timeval start, actual, times[3];
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    fd_set descriptors;
    FD_ZERO(&descriptors);
    FD_SET(sfd, &descriptors);

    Gettimeofday(&start, NULL); // start - time value for which we will compare how long we have benn waiting for a response


    while(Select(sfd + 1, &descriptors, NULL, NULL, &timeout)){  //in LINUX Select modifies timeout http://man7.org/linux/man-pages/man2/select.2.html
        struct data data;
        
        if(get_one_packet(sfd, &data)){
            if(data.id == getpid() && data.seq / 3 == ttl){ // checking id it is our packet
                if(data.type == ICMP_ECHOREPLY)
                    arrived = 1;
                
                Gettimeofday(&actual, NULL);    // actual -time of receiving the packet

                ip_address[reached] = data.ip;
                timersub(&actual, &start, &times[reached]);

                reached++;
                if(reached == 3)
                    break;
            }
        }
        FD_ZERO(&descriptors);
        FD_SET(sfd, &descriptors);
    }

    output(ttl, reached, ip_address, times);

    for(int it = 0; it < reached; it++) // freeing memory after malloc in get_data (ugly)
        free(ip_address[it]);

    return arrived; 
}


void traceroute(char *ip){
    int sockfd1 = Socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    struct sockaddr_in recipient;
    set_sockaddr_in(&recipient, ip);

    int ttl = 1;
    int arrived = 0;

    while(ttl <= 30 && !arrived){
        sendpackets(sockfd1, ttl, &recipient);
        arrived = receivepackets(sockfd1, ttl);
        ttl++;
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/param.h>
#include <sys/file.h>

#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include<string.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>


#define SECONDS_PER_DAY 86400
#define ICMP_TIME_REQUEST_PACKET_SIZE 20
#define IP_TIME_REPLY_PACKET_SIZE 40
#define MAGIC_SEQUENCE_NUMBER 10013


long send_icmp_ts_request(int sockfd, struct sockaddr* connection);
long recv_icmp_ts_reply(int sockfd, long ts_orig);
long process_recv_data(char* data, int count, struct sockaddr_in* from, long ts_orig);
u_short in_cksum(u_short* data, int len);
int protonumber_by_name(char* proto_name);


int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Few arguments: Destination address is not specified\n");
        return 1;
    }

    char* dest_ip_addr = argv[1];

    struct sockaddr dest_sockaddr;
    memset(&dest_sockaddr, 0, sizeof(struct sockaddr));

    struct sockaddr_in* dest_sockaddr_in = (struct sockaddr_in*)&dest_sockaddr;
    dest_sockaddr_in->sin_family = AF_INET;
    if (inet_aton(dest_ip_addr, &(dest_sockaddr_in->sin_addr)) == 0) {
        printf("Incorrect destination address: %s\n", dest_ip_addr);
        return 1;
    }

    int icmp_proto = protonumber_by_name("icmp");
    assert(icmp_proto == IPPROTO_ICMP);

    int sockfd = socket(AF_INET, SOCK_RAW, icmp_proto);
    if (sockfd < 0) {
        perror(strerror(errno));
        return 1;
    }

    long ts_orig = send_icmp_ts_request(sockfd, &dest_sockaddr);
    long ts_recv = recv_icmp_ts_reply(sockfd, ts_orig);

    long ts_diff_ms = ts_recv - ts_orig;

    if (ts_diff_ms < 0) {
        printf("Incorrect time difference: %li \n", ts_diff_ms);
        exit(1);
    }

    printf("Time diff = %li ms \n", ts_diff_ms);

    return 0;
}

long send_icmp_ts_request(int sockfd, struct sockaddr* connection)
{
    int packet_size = ICMP_TIME_REQUEST_PACKET_SIZE;

    char* packet = (char*)malloc(packet_size);
    struct icmp* icp = (struct icmp*)packet;

    icp->icmp_type = ICMP_TSTAMP;
    icp->icmp_code = 0;
    icp->icmp_cksum = 0;
    icp->icmp_id = getpid();
    icp->icmp_seq = MAGIC_SEQUENCE_NUMBER;

    struct timeval t_orig;
    gettimeofday(&t_orig, (struct timezone*) NULL);
    long ts_orig = (t_orig.tv_sec % SECONDS_PER_DAY) * 1000 + t_orig.tv_usec / 1000;
    icp->icmp_otime = htonl(ts_orig);
    icp->icmp_rtime = 0;
    icp->icmp_ttime = 0;

    icp->icmp_cksum = in_cksum((u_short*)icp, packet_size);

    int c_count = sendto(sockfd,
                             packet,
                             packet_size,
                             0,
                             connection,
                             sizeof(struct sockaddr));

    if (c_count < 0) {
        perror(strerror(errno));
        exit(1);
    }
    else if (c_count != packet_size) {
        printf("Not all data was sended: %d \n", c_count);
        exit(1);
    }

    return ts_orig;
}

long recv_icmp_ts_reply(int sockfd, long ts_orig)
{
    int packet_size = IP_TIME_REPLY_PACKET_SIZE;

    char* packet = (char*)malloc(packet_size);

    fd_set read_fd_set;
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    FD_ZERO (&read_fd_set);
    FD_SET (sockfd, &read_fd_set);

    struct sockaddr_in from;
    int count;
    uint fromlen;

    for (size_t i = 0; i < 5; ++i) {
        if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, &timeout) == 0) {
            continue;
        }

        if (FD_ISSET(sockfd, &read_fd_set)) {
            count = recvfrom(sockfd,
                             (char*) packet,
                             packet_size,
                             0,
                             (struct sockaddr*)&from,
                             &fromlen);

            if (count < 0) {
                perror(strerror(errno));
                continue;
            }

            long ts_recv = process_recv_data((char*)packet, count, &from, ts_orig);
            if (ts_recv < 0) {
                continue;
            }

            return ts_recv;
        }
    }

    printf("No packets recieve\n");
    exit(1);
}

long process_recv_data(char* data, int len, struct sockaddr_in* from, long ts_orig)
{
    struct ip* ip;
    struct icmp* icp;
    int icmp_offset;
    size_t icmp_size;

    ip = (struct ip*)data;
    icmp_offset = ip->ip_hl << 2;

    icp = (struct icmp*)(data + icmp_offset);
    icmp_size = len - icmp_offset;

    if (icmp_size < ICMP_TIME_REQUEST_PACKET_SIZE) {
        printf("ICMP packet too short: size = %zu\n!. Source: %s", icmp_size, inet_ntoa(from->sin_addr));
        exit(1);
    }

    if (icp->icmp_type == ICMP_TSTAMPREPLY) {
        if (ntohl(icp->icmp_otime) != ts_orig) {
            printf("Original timestamp is not identical!\n");
            exit(1);
        }

        if (icp->icmp_id != getpid()) {
            printf("Identifier is not identical!\n");
            exit(1);
        }

        if (icp->icmp_seq != MAGIC_SEQUENCE_NUMBER) {
            printf("Sequence number is not identical!\n");
            exit(1);
        }

        long ts_recv = ntohl(icp->icmp_rtime);

        return ts_recv;
    }

    return -1;
}

int protonumber_by_name(char* proto_name)
{
    struct protoent* proto;
    proto = getprotobyname(proto_name);
    if (proto == NULL) {
        printf("Couldn't get ICMP protocol number\n");
        exit(1);
    }

    return proto->p_proto;
}

u_short in_cksum(u_short* data, int len)
{
    register int nleft = len;
    register int sum = 0;
    register u_short* w = data;
    u_short answer = 0;

    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1) {
        *(u_char*)(&answer) = *(u_char*)w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return (answer);
}

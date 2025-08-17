
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/time.h>

#include <arpa/inet.h>

#include <netinet/ip_icmp.h>
#include <netinet/ip.h>

#include "packets.h"

static char * adresses[PACKETS_PER_TTL];
static uint8_t addr_index = 0;

void send_icmp_echo_request(int sockfd, int ttl, int n) 
{
    if(n == 0) {
        addr_index = 0;
    }
    struct icmp header;
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_hun.ih_idseq.icd_id = pid_id;
    header.icmp_hun.ih_idseq.icd_seq = sequence + n;
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum((u_int16_t*)&header, sizeof(header));

    struct sockaddr_in recipient;
    bzero(&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    int er = inet_pton(AF_INET, dest_ip, &recipient.sin_addr);

    if(er == 0) {
        printf("Invalid ip — could not proceed with the request\n");
        exit(1);
    }
    else if(er == -1){
        perror(NULL);
        exit(1);
    }
    
    if(setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)) == -1) {
        perror("An error occured while trying to set socket options");
        exit(1);
    }

    if(gettimeofday(&rtt[(sequence + n) % PACKETS_PER_TTL], NULL) == -1) {
        perror("An error occured while getting time of day");
        exit(1);
    }

    ssize_t bytes_sent = sendto (
        sockfd,
        &header,
        sizeof(header),
        0,
        (struct sockaddr*)&recipient,
        sizeof(recipient)
    );

    if(bytes_sent == -1) {
        perror("An error occured while trying to send the packet");
        exit(1);  
    }
}


int read_awaiting_packet(int sockfd) 
{
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    u_int8_t buffer[IP_MAXPACKET];

    ssize_t packet_len = recvfrom(
                sockfd,
                buffer,
                IP_MAXPACKET,
                MSG_DONTWAIT,
                (struct sockaddr*)&sender,
                &sender_len
            );

    if(packet_len == -1) {
        perror("An error occured while recieving a packet");
        exit(1);
    }        

    int valid_packet = check_response(buffer);

    if(!valid_packet){
        return 0;
    }
    
    char sender_ip[20];

    const char * result = inet_ntop (
        AF_INET,
        &(sender.sin_addr),
        sender_ip,
        sizeof(sender_ip)
    );

    if(result == NULL) {
        perror("An error occured while extracting response address");
        exit(1);
    }

    print_sender_ip(sender_ip);

    if(strcmp(sender_ip, dest_ip) == 0){
        end = 1;
    }

    return 1;
}

int check_response(u_int8_t *raw_response)
{
    struct ip* ip_header = (struct ip*) raw_response;
    u_int8_t* icmp_packet = raw_response + 4 * ip_header->ip_hl;
    struct icmp* icmp_header = (struct icmp*) icmp_packet;

    if(icmp_header->icmp_type == ICMP_TIME_EXCEEDED){

        struct ip* ip_header_inside = (struct ip*) (icmp_packet + 8);
        u_int8_t* icmp_packet_inside = icmp_packet + 8 + (4 * ip_header_inside->ip_hl);
        struct icmp* icmp_header_inside = (struct icmp*) icmp_packet_inside;

        return record_rtt(icmp_header_inside->icmp_seq, icmp_header_inside->icmp_id);
    }
    else if(icmp_header->icmp_type == ICMP_ECHOREPLY) {
        return record_rtt(icmp_header->icmp_seq, icmp_header->icmp_id);
    }
    
    return 0;
}

int record_rtt(uint16_t res_seq, uint16_t res_id) 
{
    if(res_seq >= sequence && res_seq < sequence + PACKETS_PER_TTL && res_id == pid_id){
        struct timeval rec_timestamp;
        if(gettimeofday(&rec_timestamp, NULL) == -1){
            perror("An error occured while getting time of day");
            exit(1);
        };
        timersub(&rec_timestamp, &rtt[res_seq % PACKETS_PER_TTL], &rtt[res_seq % PACKETS_PER_TTL]);

        return 1;
    }
    return 0;
}

void print_sender_ip(char * sender_ip) 
{
    for(int i=0; i < addr_index; i++) {
        if(strcmp(sender_ip, adresses[i]) == 0) {
            return;
        }
    }
    adresses[addr_index++] = sender_ip;
    printf("%s ", sender_ip);
}

u_int16_t compute_icmp_checksum (const void *buff, int length)
{
	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

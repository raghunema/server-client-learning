#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT "5234" //port to run client on

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "IP address required!\n");
        return EXIT_FAILURE;
    }

    struct addrinfo hints, *server_result, *pointer;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; //don't care for IPv4 of IPv6
    hints.ai_socktype = SOCK_STREAM; //Want TCP connection

    int addrInfo_status = getaddrinfo(argv[1], SERVER_PORT, &hints, &server_result); //get address info of server
    if (addrInfo_status != 0) {
        fprintf(stderr, "Unable to get address info of specific host:%s\n", gai_strerror(addrInfo_status));
        return EXIT_FAILURE;
    };

    //loop through all the address options available and try to bind to one
    int client_socket;
    void *addr;
    for (pointer = server_result; pointer != NULL; pointer = pointer->ai_next) {
        struct sockaddr_in *ipv4;
        struct sockaddr_in6 *ipv6;

        //need to make it in the for loop since we don't know what type the sever socket will be
        client_socket = socket(pointer->ai_family, pointer->ai_socktype, pointer->ai_protocol);
        if (client_socket < 0) {
            printf("Unable to create socket with correct specifications\n");
            continue;
        } 
       
        if (pointer->ai_family == AF_INET) {
            ipv4 = (struct sockaddr_in*) pointer->ai_addr;
            addr = &(ipv4->sin_addr);
            char ipstring[INET6_ADDRSTRLEN];
            inet_ntop(pointer->ai_family, addr, ipstring, INET6_ADDRSTRLEN);
            printf("Address of to host: %s (type IPv4)\n", ipstring);
        } else {
            ipv6 = (struct sockaddr_in6*) pointer->ai_addr;
            addr = &(ipv6->sin6_addr);
            char ipstring[INET6_ADDRSTRLEN];
            inet_ntop(pointer->ai_family, addr, ipstring, INET6_ADDRSTRLEN);
            printf("Address of host: %s (type IPv6)\n", ipstring);
        }

        int connect_status = connect(client_socket, pointer->ai_addr, pointer->ai_addrlen);
        if (connect_status < 0) {
            printf("Unable to connect to host, trying next address\n");
            close(client_socket);
            continue;
        }

        break;
    }

    if (pointer == NULL) { //run out of address to try
        fprintf(stderr, "Unable to find an address to connect to\n");
        return EXIT_FAILURE;
    }

    freeaddrinfo(server_result);
    char recv_msg_buffer[1000];
    int recv_msg_len = recv(client_socket, recv_msg_buffer, sizeof(recv_msg_buffer)-1, 0); //why the -1 on size, for null terminator?
    if (recv_msg_len < 0) {
        fprintf(stderr, "Unable to acknowledge connection\n");
        return EXIT_FAILURE;
    }

    recv_msg_buffer[recv_msg_len] = '\0';
    printf("Acknowledgement messag from server:\n\t%s", recv_msg_buffer);

    close(client_socket);

    return EXIT_SUCCESS;
}
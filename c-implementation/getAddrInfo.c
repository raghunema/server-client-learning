//Implementation of beej's get socket addr function

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main(int argc, char *args[]) {
    struct addrinfo hints; //not sure why its called hints but ok
    struct addrinfo *result = NULL; //pointer to a new addressInfo object that is written out
    struct addrinfo *pointer = NULL; //pointer to traverse the list

    //set the hints to 0?
    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = AF_UNSPEC; //don't care about IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; //setting to TCP instead of UDP


    if (argc != 2) {
        printf("Please use in this manner: ./server hostname\n");
        return EXIT_FAILURE;
    }

    int status = getaddrinfo(args[1], NULL, &hints, &result);
    //1. hostname/domain
    //2. port number - don't care
    //3. configuration? i.e. what type of connection
    //4. getaddrinfo wants a pointer pointer for the result??

    if (status != 0) {
        fprintf(stderr, "Unable to get address of host, error:%s\n", gai_strerror(status));
        return EXIT_FAILURE;
    }

    printf("Showing address info for ip: %s\n", args[1]);

    for (pointer = result; pointer != NULL; pointer = pointer->ai_next) {
        char *ipversion;
        void *addr;
        struct sockaddr_in *ipv4;
        struct sockaddr_in6 *ipv6;

        //parse?
        if (pointer->ai_family == AF_INET) {  //IPv4
            ipversion = "IPv4";
            ipv4 = (struct sockaddr_in *) pointer->ai_addr; //cast to IPv4 address type (pointer)
            addr = &(ipv4->sin_addr); //sin addr is a struct for in_addr, don't want the struct want the pointer to addr
        } else { //IPv6
            ipversion = "IPv6";
            ipv6 = (struct sockaddr_in6 *) pointer->ai_addr;
            addr = &(ipv6->sin6_addr); //
        }

        //convert the address to readable string?
        //use the pointer of the address to write a string
        char ipstring[INET6_ADDRSTRLEN];
        inet_ntop(pointer->ai_family, addr, ipstring, INET6_ADDRSTRLEN);

        printf("Address of hostname: %s (type %s)\n", ipstring, ipversion);

    }

    freeaddrinfo(result);

    return EXIT_SUCCESS;
}
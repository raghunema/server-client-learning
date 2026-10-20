#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

//Responsible for opening up a socket, binding to a port, and listening
//On recieve, it will print out a simple print
//and send a response back (?)

//1. Get address 
//2. Open socket
//3. Bind Socket to port
//4. Handle a recieve
//5. Handle a send?

#define MY_PORT 5234 //should be free on beelink

int main(int argc, char *args[]) {

    if (argc != 2) {
        printf("Please use in this manner: ./server hostname\n");
        return EXIT_FAILURE;
    }

    struct addrinfo hints, *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; //don't care for IPv4 of IPv6
    hints.ai_socktype = SOCK_STREAM; //Want TCP connection
    hints.ai_flags = AI_PASSIVE; //not entirely sure what passive does
    


    getaddrinfo(NULL, MY_PORT, &hints, result);

    int socketfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);


    return EXIT_SUCCESS;
}
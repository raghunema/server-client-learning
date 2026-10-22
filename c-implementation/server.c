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

#define MY_PORT "5234" //should be free on beelink
#define BACKLOG 2 //how many connections it can try to hold at once in queue (so 3 in total)?

int main(void) {

    struct addrinfo hints, *result;
    struct sockaddr_storage client_socket; //don't know what socket they will have
    socklen_t client_socket_size;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; //don't care for IPv4 of IPv6
    hints.ai_socktype = SOCK_STREAM; //Want TCP connection
    hints.ai_flags = AI_PASSIVE; //not entirely sure what passive does

    int addrInfo_status = getaddrinfo(NULL, MY_PORT, &hints, &result);
    if (addrInfo_status != 0) {
        fprintf(stderr, "Unable go get address info: %s\n", gai_strerror(addrInfo_status));
        return EXIT_FAILURE;
    }

    //use the filled in information from results
    int socketfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (socketfd == -1) {
        perror("Unable go get socket\n");
        close(socketfd);
        return EXIT_FAILURE;
    }
    
    int bind_status = bind(socketfd, result->ai_addr, result->ai_addrlen);
    if (bind_status != 0){
        perror("Unable to bind to socket\n");
        close(socketfd);
        return EXIT_FAILURE;
    }

    int listen_status = listen(socketfd, BACKLOG);
    if (listen_status != 0){
        perror("Unable to start listening\n");
        close(socketfd);
        return EXIT_FAILURE;
    }
    printf("Listening on port %s\n", MY_PORT);
    //add error check
    client_socket_size = sizeof(client_socket);

    while(1) {
        int new_socketfd = accept(socketfd, (struct sockaddr *)&client_socket, &client_socket_size);
        if (new_socketfd < 0) {
            fprintf(stdout, "Rejecting connection"); //because we may have more than capacity? so don't auto close
        }

        //do i need this if i am already accepting the information in the client socket?
        //getpeername(new_socketfd, (struct sockaddr *)&client_socket, &client_socket_size);
        
        
        char *new_connection_msg = "New Connection accepted!";
        int msg_len = strlen(new_connection_msg);
        int sent_size = send(new_socketfd, new_connection_msg, msg_len, 0); //last var is for flags, 0 for now (don't know what it does)
        if (sent_size != msg_len) {
            fprintf(stderr, "Send failure");
            return EXIT_FAILURE;
        }


    }

    freeaddrinfo(result);

    return EXIT_SUCCESS;
}
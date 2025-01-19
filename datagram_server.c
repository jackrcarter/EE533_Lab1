/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include<signal.h> // needed for the SIGCHLD

// need to implement this function
/*
void dostuff(int sockfd){
    char buffer[256];
    int n;

    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
        printf("Here is the message: %s\n",buffer);
    n = write(sockfd,"I got your message",18);
        if (n < 0) error("ERROR writing to socket");
}
*/
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    // Create the Socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
       error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the Socket
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0) 
             error("ERROR on binding");

    clilen = sizeof(cli_addr);
    
    
    while (1)
    {
        bzero(buffer, 256);
        n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *) &cli_addr, &clilen);
        if (n < 0)
            error("ERROR on recvfrom");

        printf("Here is the message: %s\n", buffer);
        
        // Send a response datagram
        n = sendto(sockfd, "I got your message", 18, 0, (struct sockaddr *) &cli_addr, clilen);
        if (n < 0)
            error("ERROR on sendto");
            
    } /* end of while */

    
    return 0; 
}

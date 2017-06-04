#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void ErrorExit(const char *error){

    perror(error);
    exit(1);
}

int main(void)
{
    int serverSockFd, clientSockFd;
    socklen_t serverLen, clientLen;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    struct ip_mreq mreq;

    char strServer[50] = "";
    char strClient[50] = "Server1 message";
    int strServerLen = 0;
    int strClientLen = sizeof(strClient);
    int isMulticast = 1;
    
    if((serverSockFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        ErrorExit("socket()");

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(9734);
    serverLen = sizeof(serverAddress);
    clientLen = sizeof(clientAddress);

    if(setsockopt(serverSockFd, SOL_SOCKET, SO_REUSEADDR, &isMulticast, sizeof(int)) < 0)
        ErrorExit("setsockopt1");

    if(bind(serverSockFd, (struct sockaddr *)&serverAddress, serverLen) < 0)
        ErrorExit("bind()");

    mreq.imr_multiaddr.s_addr = inet_addr("225.0.0.37");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if(setsockopt(serverSockFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
        ErrorExit("setsockopt2");

    while(1){

        if (recvfrom(serverSockFd, strServer, sizeof(strServer), 0, (struct sockaddr *) &clientAddress, &clientLen) == -1)
            ErrorExit("recvfrom()");         

        printf("Message from: %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
        printf("Message: %s\n" , strServer);
         
        if (sendto(serverSockFd, strClient, strClientLen, 0, (struct sockaddr*) &clientAddress, clientLen) == -1)
            ErrorExit("sendto()");
    }
 
    close(serverSockFd);
    return 0;
}
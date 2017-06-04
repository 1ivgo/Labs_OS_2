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

    char strServer[50] = "Client message";
    char strClient[50] = "";
    int strServerLen = sizeof(strServer);
    int strClientLen = 0;
    int isMulticast = 1;

    if((clientSockFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        ErrorExit("socket()");

    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = inet_addr("225.0.0.37");
    clientAddress.sin_port = htons(9734);
    serverLen = sizeof(serverAddress);
    clientLen = sizeof(clientAddress);

    if (sendto(clientSockFd, strServer, strServerLen, 0, (struct sockaddr*) &clientAddress, clientLen) == -1)
        ErrorExit("sendto()");

    for(int i = 0; i < 2; i++){
        if (recvfrom(clientSockFd, strClient, sizeof(strClient), 0, (struct sockaddr *) &serverAddress, &serverLen) == -1)
            ErrorExit("recvfrom()");

        printf("Message from: %s:%d\n", inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));
        printf("Message: %s\n" , strClient);
    }
 
    close(clientSockFd);
    return 0;
}
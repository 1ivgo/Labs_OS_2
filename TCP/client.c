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

int main(){

	int clientSockFd;
    socklen_t clientLen;
    struct sockaddr_in clientAddress;
 
    char strServer[30] = "Client also was there!";
    char strClient[30] = "";

    clientSockFd = socket (AF_INET, SOCK_STREAM, 0);
    
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr= inet_addr("127.0.0.1");
    clientAddress.sin_port = htons(9734);
    clientLen = sizeof(clientAddress);
    
    if(connect(clientSockFd, (struct sockaddr *) &clientAddress, clientLen) < 0)
    	perror("Ошибка подключения");
    
    write(clientSockFd, strServer, sizeof(strServer));
    read(clientSockFd, strClient, sizeof(strClient));
    printf("%s\n", strClient);
    close(clientSockFd);

	return 0;
}
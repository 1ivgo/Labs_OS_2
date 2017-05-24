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

	int serverSockFd, clientSockFd;
	socklen_t serverLen, clientLen;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;

	if((serverSockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		ErrorExit("Ошибка сокета");

	serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(9734);
    serverLen = sizeof(serverAddress);

    if(bind(serverSockFd, (struct sockaddr *)&serverAddress, serverLen) < 0)
    	ErrorExit("Ошибка присвоения имени");

    if(listen(serverSockFd, 1) < 0)
    	ErrorExit("Ошибка listen");

    while(1){

    	char strClient[30] = "Server was there!";
    	char strServer[30] = "";

    	clientLen = sizeof(clientAddress);
        if((clientSockFd = accept(serverSockFd, (struct sockaddr *) &clientAddress, &clientLen)) < 0)
        	ErrorExit("Ошибка принятия связи на сокет");
 
        read(clientSockFd, strServer, sizeof(strServer));
    	printf("Server:%s\n", strServer);
        write(clientSockFd, strClient, sizeof(strClient));
        close(clientSockFd);
    }

	return 0;
}
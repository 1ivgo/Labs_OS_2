#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>

void ErrorExit(const char *error){

    perror(error);
    exit(1);
}

int main(){

    int serverSockFd, clientSockFd;
    socklen_t serverLen, clientLen;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    int fd, size, status;
    char* mem;

    if((serverSockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        ErrorExit("socket()");

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(9734);
    serverLen = sizeof(serverAddress);

    if(bind(serverSockFd, (struct sockaddr *)&serverAddress, serverLen) < 0)
        ErrorExit("bind()");

    if(listen(serverSockFd, 1) < 0)
    ErrorExit("listen()");

    clientLen = sizeof(clientAddress);
    if((clientSockFd = accept(serverSockFd, (struct sockaddr *) &clientAddress, &clientLen)) < 0)
        ErrorExit("accept()");

    if((fd = open("File.txt", O_CREAT | O_EXCL | O_WRONLY, 0666)) < 0)
        ErrorExit("open()");
    recv(clientSockFd, &size, sizeof(int), 0);
    printf("Size of file: %d\n", size);
    mem = (char*)malloc(size);
    if(mem == NULL)
        ErrorExit("malloc()");
    recv(clientSockFd, mem, size, 0);
    printf("File is here\n");
    status = write(fd, mem, size);
    send(clientSockFd, &status, sizeof(int), 0);
    printf("Writed: %d\n", status);

    close(fd);
    close(clientSockFd);

    return 0;
}

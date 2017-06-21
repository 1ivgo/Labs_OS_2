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

    int clientSockFd;
    socklen_t clientLen;
    struct sockaddr_in clientAddress;
    struct stat obj;
    int fd, size, status;

    clientSockFd = socket (AF_INET, SOCK_STREAM, 0);

    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr= htonl(INADDR_ANY);
    clientAddress.sin_port = htons(9734);
    clientLen = sizeof(clientAddress);

    if((fd = open("File.txt", O_RDONLY)) < 0)
        ErrorExit("open()");

    if(connect(clientSockFd, (struct sockaddr *) &clientAddress, clientLen) < 0)
        perror("connect()");

    stat("File.txt", &obj);
    size = obj.st_size;
    send(clientSockFd, &size, sizeof(int), 0);
    printf("Size of file: %d\n", size);
    sendfile(clientSockFd, fd, NULL, size);
    printf("File sended\n");
    recv(clientSockFd, &status, sizeof(int), 0);

    if(status)
        printf("Successfully, writed:%d\n", status);
    else
        printf("Failed\n");

    close(fd);
    close(clientSockFd);

    return 0;
}

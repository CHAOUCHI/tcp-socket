#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define PORT 2000
int main(){

    int client_fd = socket(AF_INET,SOCK_STREAM,0);
    perror("socket");

    struct sockaddr_in addr = {
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port =htons(PORT),
        .sin_family = AF_INET
    };

    bind(client_fd,(struct sockaddr*)&addr,sizeof(addr));
    perror("bind");

     struct sockaddr_in serv_addr = {
        .sin_addr.s_addr = inet_addr("127.0.0.1"),
        .sin_port =htons(3000),
        .sin_family = AF_INET
    };
    connect(client_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    perror("connect");

    char buf[BUFSIZ];memset(buf,0,BUFSIZ);
    strcpy(buf,"GET /index.html HTTP/1.1");
    send(client_fd,buf,strlen(buf),0);
    perror("send");

    close(client_fd);

}
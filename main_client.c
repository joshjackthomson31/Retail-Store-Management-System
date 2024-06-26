#include "client.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>

int main()
{
    struct sockaddr_in serv;
    int sd;
    char buf[100];
    sd = socket(AF_UNIX,SOCK_STREAM,0);         //domain,type,protocol

    serv.sin_family = AF_UNIX;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(5658);

    connect(sd,(void *)&serv,sizeof(serv));
    menu(sd);
}
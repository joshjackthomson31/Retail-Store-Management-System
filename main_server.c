#include "server.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>

int main()
{
   struct sockaddr_in serv,cli;
    int sd,sz,nsd;
    sd = socket(AF_UNIX,SOCK_STREAM,0);

    char buf[80];

    serv.sin_family = AF_UNIX;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(5658);
    
    bind(sd,(void *)(&serv),sizeof(serv));

    listen(sd,5);

    

    while(1)
    {
        socklen_t sz = sizeof(cli);
        int nsd = accept(sd,(void *)&cli,&sz);
        write(1,"Waiting for client....\n",sizeof("Waiting for client....\n"));
        if (!fork())
        {
            close(sd);
            readFromClient(nsd);
            close(nsd);
            return 0;
        }
        else
        {
            close(nsd);
        }
    }
}
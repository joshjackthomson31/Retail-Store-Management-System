#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "data.h"

int main()
{
    int fd = open("products.txt",O_TRUNC | O_CREAT | O_RDWR,0777);
    struct Product p;
    strcpy(p.pname,"Apple");
    p.productId = 1;
    p.price = 20;
    p.quantity = 5;
    write(fd,&p,sizeof(p));
    strcpy(p.pname,"Mango");
    p.productId = 2;
    p.price = 30;
    p.quantity = 4;
    write(fd,&p,sizeof(p));
    strcpy(p.pname,"PineApple");
    p.productId = 3;
    p.price = 40;
    p.quantity = 7;
    write(fd,&p,sizeof(p));
    strcpy(p.pname,"Pomegranate");
    p.productId = 4;
    p.price = 50;
    p.quantity = 6;
    write(fd,&p,sizeof(p));
    // struct Product p1;
    // lseek(fd,0,SEEK_SET);
    // read(fd,&p1,sizeof(p1));
    // printf("%d",p1.price);
    return 0;
}
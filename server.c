#include "server.h"
#include "data.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int cart_q[1000];
int cart_prodid[1000];
int size=0;

void readFromClient(int socket_descriptor)
{
    int choice;
    //printf("Hi");
    while(1)
    {
        printf("Reading choice from socket descriptor\n");
        read(socket_descriptor,&choice,sizeof(choice));
        //printf("%d\n",choice);  
        if (choice == 4)
        {
            displayProd(socket_descriptor);
            //choice = 0;
        }
        else if (choice == 1)
        {
            addProdToCart(socket_descriptor);
            //choice = 0;
        }
        else if (choice == 2)
        {
            viewProdCart(socket_descriptor);
            //choice = 0;
        }
        else if (choice == 3)
        {
            editProdCart(socket_descriptor);
            //choice = 0;
        }
        else if(choice == 5)
        {
            confirmProdOrder(socket_descriptor);
        }
        else if(choice == 101)
        {
            addProduct(socket_descriptor);
        }
        else if(choice == 102)
        {
            deleteProduct(socket_descriptor);
        }
        else if(choice == 103)
        {
            //printf("Hello anna");
            modifyProduct(socket_descriptor);
        }
    }
}

void confirmProdOrder(int socket_descriptor)
{
    struct Product p; int cost = 0;
    int fd = open("products.txt",O_RDWR,0777);
    int check = 1;
    //checking if all products are present
    // for (int j=0;j<size;j++)
    // {
    //     lseek(fd,0,SEEK_SET);int y=1;
    //     while(1)
    //     {
    //         int n = read(fd,&p,sizeof(p));
    //         if (n<=0)
    //         {
    //             break;
    //         }
    //         else
    //         {
    //             if (p.productId == cart_prodid[j])
    //             {
    //                 if (p.quantity < cart_q[j])
    //                 {
    //                     check = 0;
    //                 }
    //                 y = 0;
    //             }
    //         }
    //     }  
    //     if (y == 1)
    //     {
    //         check = 0;
    //     }
    // }
    // write(socket_descriptor,&check,sizeof(check));
    // if (check == 0)
    // {
    //     return ;
    // }
    struct flock lock;cost = 0;
    //locking
    for (int j=0;j<size;j++)
    {
        lseek(fd,0,SEEK_SET);
        while(1)
        {
            int n = read(fd,&p,sizeof(p));
            if (n<=0)
            {
                break;
            }
            else
            {
                if (p.productId == cart_prodid[j])
                {
                    lock.l_type = F_WRLCK;
                    lock.l_pid = getpid();
                    lock.l_whence = SEEK_CUR;
                    lock.l_start = -sizeof(p);
                    lock.l_len = sizeof(p);
                    fcntl(fd,F_SETLKW,&lock);
                    cost += cart_q[j] * p.price;
                }
            }
        }  
    }
    //critical section
    //checking if all products are present
    for (int j=0;j<size;j++)
    {
        lseek(fd,0,SEEK_SET);int y=1;
        while(1)
        {
            int n = read(fd,&p,sizeof(p));
            if (n<=0)
            {
                break;
            }
            else
            {
                if (p.productId == cart_prodid[j])
                {
                    if (p.quantity < cart_q[j])
                    {
                        check = 0;
                    }
                    y = 0;
                }
            }
        }  
        if (y == 1)
        {
            check = 0;
        }
    }
    write(socket_descriptor,&check,sizeof(check));
    int amt_paid;
    write(socket_descriptor,&cost,sizeof(cost));
    read(socket_descriptor,&amt_paid,sizeof(amt_paid));
    if (check == 0)
    {
        write(socket_descriptor,"Failed because of insufficiency of products\n",sizeof("Failed because of insufficiency of products\n"));
        lseek(fd,0,SEEK_SET);
        //unlock
        for (int j=0;j<size;j++)
        {
            lseek(fd,0,SEEK_SET);
            while(1)
            {
                int n = read(fd,&p,sizeof(p));
                if (n<=0)
                {
                    break;
                }
                else
                {
                if (p.productId == cart_prodid[j])
                {
                        lock.l_type = F_UNLCK;
                        lock.l_pid = getpid();
                        lock.l_whence = SEEK_CUR;
                        lock.l_start = -sizeof(p);
                        lock.l_len = sizeof(p);
                        fcntl(fd,F_SETLK,&lock);
                } 
                }
            }
        }
        return ;
    }
    if (amt_paid != cost)
    {
        write(socket_descriptor,"Failed because the required amount given is not exactly correct\n",sizeof("Failed because the required amount given is not exactly correct\n"));
        lseek(fd,0,SEEK_SET);
        for (int j=0;j<size;j++)
        {
            lseek(fd,0,SEEK_SET);
            while(1)
            {
                int n = read(fd,&p,sizeof(p));
                if (n<=0)
                {
                    break;
                }
                else
                {
                if (p.productId == cart_prodid[j])
                {
                        lock.l_type = F_UNLCK;
                        lock.l_pid = getpid();
                        lock.l_whence = SEEK_CUR;
                        lock.l_start = -sizeof(p);
                        lock.l_len = sizeof(p);
                        fcntl(fd,F_SETLK,&lock);
                } 
                }
            }
        }
        return ;
    }
    write(socket_descriptor,"Success\n",sizeof("Success\n"));
    int fd1 = open("receipt.txt",O_CREAT | O_TRUNC | O_RDWR,0777);
    for (int j=0;j<size;j++)
    {
        lseek(fd,0,SEEK_SET);
        int r=0;
        while(1)
        {
            int n = read(fd,&p,sizeof(p));
            if (n<=0)
            {
                break;
            }
            else
            {
               if (p.productId == cart_prodid[j])
               {
                    p.quantity = p.quantity - cart_q[j];
                    lseek(fd,-sizeof(p),SEEK_CUR);
                    write(fd,&p,sizeof(p));
                    cart_q[j] = 0;
                    //writing receipt
                    write(fd1,&p.productId,sizeof(p.productId));
                    write(fd1,p.pname,sizeof(p.pname));
                    write(fd1,&p.price,sizeof(p.price));
                    write(fd1,&cart_q[j],sizeof(cart_q[j]));
               } 
            }
        }
    }  
    lseek(fd,0,SEEK_SET);
    for (int j=0;j<size;j++)
    {
        lseek(fd,0,SEEK_SET);
        while(1)
        {
            int n = read(fd,&p,sizeof(p));
            if (n<=0)
            {
                break;
            }
            else
            {
               if (p.productId == cart_prodid[j])
               {
                    lock.l_type = F_UNLCK;
                    lock.l_pid = getpid();
                    lock.l_whence = SEEK_CUR;
                    lock.l_start = -sizeof(p);
                    lock.l_len = sizeof(p);
                    fcntl(fd,F_SETLK,&lock);
               } 
            }
        }
    }
    for (int j=0;j<size;j++)
    {
        cart_prodid[j] = 0;
    }
    size = 0;
}

void addProdToCart(int socket_descriptor)
{
    int fd = open("products.txt",O_RDWR,0777);
    int prodid,q;
    //printf("Hello in the required place\n");
    read(socket_descriptor,&prodid,sizeof(prodid));
    read(socket_descriptor,&q,sizeof(q));
    cart_q[size] = q;
    cart_prodid[size] = prodid;
    size++;
}

void editProdCart(int socket_descriptor)
{
    int prodid,newq;
    //printf("In the plac where we need\n");
    read(socket_descriptor,&prodid,sizeof(prodid));
    read(socket_descriptor,&newq,sizeof(newq));
    for (int j=0;j<size;j++)
    {
        if (cart_prodid[j] == prodid)
        {
            cart_q[j] = newq;
        }
    }
}

void displayProd(int socket_descriptor)
{
    struct Product p;
    int fd = open("products.txt",O_RDWR,0777);
    while(1)
    {
        int n = read(fd,&p,sizeof(p));
        if (n<=0)
        {
            break;
        }
        else
        {
            //printf("Hello dude\n");
            int l=0;
            write(socket_descriptor,&l,sizeof(l));
            write(socket_descriptor,&p,sizeof(p));
            //printf("%d %s %d %d\n",p.productId,p.pname,p.price,p.quantity);
        }
    }
    int l=1;
    write(socket_descriptor,&l,sizeof(l));
}

void viewProdCart(int socket_descriptor)
{
    for (int j=0;j<size;j++)
    {
        int l=0;
        write(socket_descriptor,&l,sizeof(l));
        write(socket_descriptor,&cart_prodid[j],sizeof(int));
        write(socket_descriptor,&cart_q[j],sizeof(int));
    }   
    int l=1;
    write(socket_descriptor,&l,sizeof(l));
}

void addProduct(int socket_descriptor)
{
    struct Product p;
    int fd = open("products.txt",O_RDWR,0777);
    char pname[80];int prodid,price,q;
    read(socket_descriptor,pname,sizeof(pname));
    read(socket_descriptor,&prodid,sizeof(prodid));
    read(socket_descriptor,&price,sizeof(price));
    read(socket_descriptor,&q,sizeof(q));
    strcpy(p.pname,pname);
    p.productId = prodid;
    p.price = price;
    p.quantity = q;
    lseek(fd,0,SEEK_END);
    write(fd,&p,sizeof(p));
}

void deleteProduct(int socket_descriptor)
{
    int prodid;
    //printf("Hi");
    read(socket_descriptor,&prodid,sizeof(prodid));
    struct Product p;
    int fd = open("products.txt",O_RDWR,0777);
    while(1)
    {
        int n = read(fd,&p,sizeof(p));
        if (n<=0)
        {
            break;
        }
        else
        {
            if (p.productId == prodid)
            {
                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_pid = getpid();
                lock.l_whence = SEEK_CUR;
                lock.l_start = -sizeof(p);
                lock.l_len = sizeof(p);
                fcntl(fd,F_SETLKW,&lock);
                //write(socket_descriptor,"Confirm deletion\n",sizeof("Confirm deletion\n"));
                p.productId = -1;
                lseek(fd,-sizeof(p),SEEK_CUR);
                write(fd,&p,sizeof(p));
                lock.l_type = F_UNLCK;
                fcntl(fd,F_SETLK,&lock);
            }
        }
    }
}

void modifyProduct(int socket_descriptor)
{
    struct Product p;
    int fd = open("products.txt",O_RDWR,0777);
    int prodid,new_price=-1,newq=-1;
    read(socket_descriptor,&prodid,sizeof(prodid));
    lseek(fd,0,SEEK_SET);
    while(1)
    {
        int n = read(fd,&p,sizeof(p));
        if (n<=0)
        {
            break;
        }
        else
        {
            if (p.productId == prodid)
            {
                if (new_price == -1)
                {
                    new_price = p.price;
                }
                if (newq == -1)
                {
                    newq = p.quantity;
                }
                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_pid = getpid();
                lock.l_whence = SEEK_CUR;
                lock.l_start = -sizeof(p);
                lock.l_len = sizeof(p);
                fcntl(fd,F_SETLKW,&lock);
                //printf("Hi");
                write(socket_descriptor,"Inside the critical section\n",sizeof("Inside the critical section\n"));
                int l;
                read(socket_descriptor,&l,sizeof(l));
                if (l == 1)
                {
                    read(socket_descriptor,&new_price,sizeof(new_price));
                }
                read(socket_descriptor,&l,sizeof(l));
                if (l == 2)
                {
                    //printf("Hi macha\n");
                    read(socket_descriptor,&newq,sizeof(newq));
                }
                //printf("%d",newq);
                lseek(fd,-sizeof(p),SEEK_CUR);
                p.price = new_price;
                p.quantity = newq;
                write(fd,&p,sizeof(p));
                lock.l_type = F_UNLCK;
                fcntl(fd,F_SETLK,&lock);
            }
        }
    }  
}



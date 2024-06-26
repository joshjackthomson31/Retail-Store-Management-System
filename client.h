#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>

void menu(int socket_descriptor);
void admin_menu(int socket_descriptor);
void user_menu(int socket_descriptor);
void addToCart(int socket_descriptor);
void displayProducts(int socket_descriptor);
void viewCart(int socket_descriptor);
void editCart(int socket_descriptor);
void confirmOrder(int socket_descriptor);
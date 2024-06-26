#include "client.h"
#include "data.h"

void menu(int socket_descriptor)
{
    int a;
    printf("Enter 1 if you are an admin\n");
    printf("Enter any other number if you are a user\n");
    scanf("%d",&a);
    char pass[80];
    if (a == 1)
    {
        //int x = 1000;
        //write(socket_descriptor,&x,sizeof(x));
        printf("Enter your password\n");
        while(1)
        {
            scanf("%s",pass);
            if (strcmp(pass,"Admin@123") == 0)
                admin_menu(socket_descriptor);
            else
            {
                printf("The entered password is incorrect\n");
                printf("Enter your password again\n");
            }
        }
    }
    else
    {
        user_menu(socket_descriptor);
    }
}

void admin_menu(int socket_descriptor)
{
    printf("Enter the option which you want to do\n");
    printf("101.Add a product\n");
    printf("102.Delete a product\n");
    printf("103.Update the product\n");
    while(1)
    {
        int choice;char buf[80];
        printf("Enter the choice which you want\n");
        scanf("%d",&choice);
        if (choice == 101)
        {
            //struct Product p;
            char pname[80];
            write(socket_descriptor,&choice,sizeof(choice));
            write(1,"Enter product id\n",sizeof("Enter product id\n"));
            int prodid;
            scanf("%d",&prodid);
            //read(socket_descriptor,buf,sizeof(buf));
            int price;
            printf("Enter price of the product\n");
            scanf("%d",&price);
            printf("Enter name of the product\n");
            scanf("%s",pname);
            //printf("%s",pname);
            int q;
            printf("Enter quantity of the product\n");
            scanf("%d",&q);
            write(socket_descriptor,pname,sizeof(pname));
            write(socket_descriptor,&prodid,sizeof(prodid));
            write(socket_descriptor,&price,sizeof(price));
            write(socket_descriptor,&q,sizeof(q));
        }
        else if(choice == 102)
        {
            int prodid;char buf[80];
            write(socket_descriptor,&choice,sizeof(choice));
            printf("Enter product id of the product which you want to delete\n");
            scanf("%d",&prodid);
            write(socket_descriptor,&prodid,sizeof(prodid));
            //read(socket_descriptor,buf,sizeof(buf));
        }
        else if (choice == 103)
        {
            write(socket_descriptor,&choice,sizeof(choice));
            int prodid,q,price,l;
            printf("Enter product id of the product you want to modify\n");
            scanf("%d",&prodid);
            write(socket_descriptor,&prodid,sizeof(prodid));
            char buf[80];
            read(socket_descriptor,buf,sizeof(buf));
            // printf("%s",buf);
            printf("Enter 1 if you want to modify price\n");
            printf("Enter anything else if you don't want to modify price\n");
            scanf("%d",&l);
            write(socket_descriptor,&l,sizeof(l));
            if (l == 1)
            {
                printf("Enter modified price\n");
                scanf("%d",&price);
                write(socket_descriptor,&price,sizeof(price));
            }
            printf("Enter 2 if you want to modify quantity\n");
            printf("Enter anything else if you don't want to modify quantity\n");
            scanf("%d",&l);
            write(socket_descriptor,&l,sizeof(l));
            if (l == 2)
            {
                printf("Enter modified quantity\n");
                scanf("%d",&q);
                write(socket_descriptor,&q,sizeof(q));
            }
        }
    }
}

void user_menu(int socket_descriptor)
{
    printf("Here are the options\n");
    printf("1.Add a product to the cart\n");
    printf("2.View the cart\n");
    printf("3.Edit the cart\n");
    printf("4.Display the products\n");
    printf("5.Confirm your order\n");
    while(1)
    {
        int choice;
        printf("Enter the choice which you want\n");
        scanf("%d",&choice);
        //printf("\n%d\n",choice);
        if (choice == 1)
        {
            addToCart(socket_descriptor);
        }
        else if (choice == 2)
        {
            viewCart(socket_descriptor);
        }
        else if (choice == 3)
        {
            editCart(socket_descriptor);
        }
        else if(choice == 4)
        {
            displayProducts(socket_descriptor);
        }
        else if(choice == 5)
        {
            //printf("In the req place\n");
            confirmOrder(socket_descriptor);
        }
    }
}

void confirmOrder(int socket_descriptor)
{
    int a = 5;
    write(socket_descriptor,&a,sizeof(a));
    int check;int cost;
    read(socket_descriptor,&check,sizeof(check));
    read(socket_descriptor,&cost,sizeof(cost));
    printf("Total cost is %d\n",cost);
    printf("Enter the amount to proceed to payment\n");
    int c;
    scanf("%d",&c);
    write(socket_descriptor,&c,sizeof(c));
    char buf[80];
    read(socket_descriptor,buf,sizeof(buf));
    printf("%s",buf);
    //printf("Done");
}

void editCart(int socket_descriptor)
{
    int a=3;int prodid,newq;
    write(socket_descriptor,&a,sizeof(a));
    write(1,"Enter productid of the product in cart which you want to modify\n",sizeof("Enter productid of the product in cart which you want to modify\n"));
    scanf("%d",&prodid);
    write(1,"Enter the new quantity of the product\n",sizeof("Enter the new quantity of the product\n"));
    scanf("%d",&newq);
    write(socket_descriptor,&prodid,sizeof(prodid));
    write(socket_descriptor,&newq,sizeof(newq));
}

void displayProducts(int socket_descriptor)
{
    int a = 4;
    // printf("Hello bhai");
    struct Product p;
    write(socket_descriptor,&a,sizeof(int));
    while(1)
    {
        read(socket_descriptor,&a,sizeof(a));
        if (a == 1)
        {
            break;
        }
        else
        {
            read(socket_descriptor,&p,sizeof(p));
            if (p.productId != -1)
            printf("%d %s %d %d\n",p.productId,p.pname,p.price,p.quantity);
        }
    }
}

void addToCart(int socket_descriptor)
{
    int a = 1;
    write(socket_descriptor,&a,sizeof(a));
    int prodid,q;
    write(1,"Enter product id\n",sizeof("Enter product id\n"));
    scanf("%d",&prodid);
    write(socket_descriptor,&prodid,sizeof(prodid));
    write(1,"Enter quantity which you wanted to purchase\n",sizeof("Enter quantity which you wanted to purchase\n"));
    scanf("%d",&q);
    write(socket_descriptor,&q,sizeof(q));
}

void viewCart(int socket_descriptor)
{
    int a = 2,q,prodid;
    write(socket_descriptor,&a,sizeof(int));
    while(1)
    {
        read(socket_descriptor,&a,sizeof(a));
        if (a == 1)
        {
            break;
        }
        read(socket_descriptor,&prodid,sizeof(q));
        read(socket_descriptor,&q,sizeof(int));
        printf("Productid = %d and Quantity = %d\n",prodid,q);
    }
}



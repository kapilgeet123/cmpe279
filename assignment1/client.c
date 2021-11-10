// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080 
   
int main(int argc, char const *argv[]) 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
 //creates a stream socket in the Internet domain:
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    // contains a code for the address family.
    serv_addr.sin_family = AF_INET; 
    //The second field of serv_addr is unsigned short sin_port , which contain the port number. However, instead of simply copying the port number to this field, it is necessary to convert this to network byte order using the function htons() which converts a port number in host byte order to a port number in network byte order.
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    //The InetNtop() function converts an IPv4 or IPv6 Internet network address into a string in Internet standard format
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    { 
        printf("\nConnection Failed\n"); 
        return -1; 
    }

    send(sock, hello, strlen(hello), 0 ); 
    printf("Hello message sent\n"); 
    valread = read(sock, buffer, 1024); 
    printf("Read %d bytes: %s\n", valread, buffer);

    return 0; 
} 

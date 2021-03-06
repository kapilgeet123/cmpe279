// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/wait.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    //struct passwd *user_info = getpwnam("dell");
    struct passwd *user_info = getpwnam("nobody");
    printf("User ID: %u\n",user_info->pw_uid);
    if(user_info == NULL){
        perror("Failed to get user id of NOBODY");
    }

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address,
	sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // valread = read(new_socket, buffer, 1024);
    // printf("Read %d bytes: %s\n", valread, buffer);
    // send(new_socket, hello, strlen(hello), 0);
    // printf("Hello message sent\n");

    int pid = fork();
    if(pid==-1){
        perror("Error in creating child process");
        exit(EXIT_FAILURE);
    }
    else if(pid==0){
        printf("FROM CHILD: Inside Child Process\n");
        //dropping previleges
        if(setuid(user_info->pw_uid)<0){
             printf("Don't have sufficient privileges to change UID\n" ); 
            perror("Error in setting user id");
            exit(EXIT_FAILURE);
        }
        else{
            valread = read(new_socket, buffer, 1024);
            printf("FROM CHILD: Read %d bytes: %s\n", valread, buffer);
            send(new_socket, hello, strlen(hello), 0);
            printf("FROM CHILD: Hello message sent from user Nobody.\n");
        }
    }
    else if(pid>0){
        printf("FROM PARENT: Inside Parent Process\n");
        int child_status;
        waitpid(pid, &child_status, 0);
        if (child_status > 0){
            perror("FROM PARENT: Error in child process.");
            exit(EXIT_FAILURE);
        }
        else{
            printf("FROM PARENT: Child process connection in client.\n");
        }
    }

    return 0;
}

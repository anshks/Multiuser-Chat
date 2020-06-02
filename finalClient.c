#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <pthread.h>

#define SERVER_PATH     "/home/anshks/Desktop/server"

char msg[500];

void *recvmg(void *my_sock)
{
    int sock = *((int *)my_sock);
    int len;
    // client thread always ready to receive message
    while((len = recv(sock,msg,500,0)) > 0) {
        msg[len] = '\0';
        fputs(msg,stdout);
    }
}

int main(int argc,char *argv[]){
    pthread_t recvt;
    int len;
    int sock;
    char send_msg[500];
    struct sockaddr_un ServerIp;
    char client_name[100];
    strcpy(client_name, argv[1]);
    sock = socket( AF_UNIX, SOCK_STREAM,0);
    if (sock < 0) {
        perror("socket() failed");
    }
    memset(&ServerIp, 0, sizeof(ServerIp));
    ServerIp.sun_family = AF_UNIX;
    strcpy(ServerIp.sun_path, SERVER_PATH);
    if( (connect( sock ,(struct sockaddr *)&ServerIp,SUN_LEN(&ServerIp))) == -1 )
        perror("connect() failed");
    len = send(sock,client_name,strlen(client_name), 0);
    if(len < 0) 
        printf("username not sent \n");
    //creating a client thread which is always waiting for a message
    pthread_create(&recvt,NULL,recvmg,(void *)&sock);
    
    //ready to read a message from console
    while(fgets(msg,500,stdin) > 0) {
        strcpy(send_msg,client_name);
        strcat(send_msg,":");
        strcat(send_msg,msg);
        len = send(sock,send_msg,strlen(send_msg), 0);
        if(len < 0) 
            printf("\n message not sent \n");
    }
    
    //thread is closed
    pthread_join(recvt,NULL);
    close(sock);
    return 0;
}
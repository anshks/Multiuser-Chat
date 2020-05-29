#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <pthread.h>

#define SERVER_PATH     "/home/anshks/Desktop/server"

int clientCount = 0;
pthread_mutex_t mutex;

struct client{
	char name[50]; 
	int index;
	int sockID;
	int len;
	struct sockaddr_un clientAddr;
};

struct client Client[1024];
pthread_t thread[1024];

void sendTo(char *msg, int curr) {
	pthread_mutex_lock(&mutex);
	char *ptr;
	ptr = &msg;
	char to[50];
	while(*ptr != ':') {
		ptr ++;
	}
	ptr ++;
	while(*ptr == ' ') {
		ptr ++;
	}
	if (*ptr == '@') {
		ptr ++;
		int j = 0;
		while (*ptr != ' ') {
			to[j] = *ptr;
			j ++;
			ptr ++;
		}
		int fl = 0;
		for (int i = 0 ; i < clientCount ; i ++){
			if (strcmp(Client[i].name, to) == 0) {
				if(send(Client[i].sockID,msg,strlen(msg),0) < 0) {
	                perror("sending failure \n");
	                continue;
            	}
            	fl =1;
			}
		}
		if (fl == 0) {
			strcpy(msg, "No such user exists \n");
			if(send(curr,msg,strlen(msg),0) < 0) {
	            perror("sending failure \n");
	       	}
		}
	}
	else {
		for(int j = 0; j < clientCount; j++) {
	        if(Client[j].sockID != curr) {
	            if(send(Client[j].sockID,msg,strlen(msg),0) < 0) {
	                perror("sending failure \n");
	                continue;
	            }
	        }
    	}
	}
	pthread_mutex_unlock(&mutex);
}

void *recvmg(void * ClientDetail){
    struct client* clientDetail = (struct client*) ClientDetail;
    int index = clientDetail -> index;
	int clientSocket = clientDetail -> sockID;
    char msg[500];
    int len;
    //printf("%d\n", clientSocket);
    if ((len = recv(clientSocket,msg,500,0)) > 0) {
    	strcpy(Client[index].name, msg);
    }
    printf("Client %s connected.\n", msg);
    while((len = recv(clientSocket,msg,500,0)) > 0) {
        msg[len] = '\0';
        sendTo(msg,clientSocket);
    }   
}

int main(){
    struct sockaddr_un ServerIp;
    int sock=0;
    memset(&ServerIp, 0, sizeof(ServerIp));
    ServerIp.sun_family = AF_UNIX;
    strcpy(ServerIp.sun_path, SERVER_PATH);
    sock = socket( AF_UNIX, SOCK_STREAM,0);
    if (sock < 0) {
        perror("socket() failed");
    }
    if( bind( sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp)) == -1 )
        perror("bind() failed");
    else
        printf("Server Started\n");
        
    if( listen( sock ,20 ) == -1 )
        perror("listen() failed");
        
    while(1){
        Client[clientCount].sockID = accept(sock, (struct sockaddr *) &Client[clientCount].clientAddr, &Client[clientCount].len);
        Client[clientCount].index = clientCount;
        pthread_mutex_lock(&mutex);
        // creating a thread for each client 
        pthread_create(&thread[clientCount],NULL,recvmg,(void *)&Client[clientCount]);
        clientCount++;
        pthread_mutex_unlock(&mutex);
    }
    for(int i = 0 ; i < clientCount ; i ++)
		pthread_join(thread[i],NULL);
    return 0; 
}
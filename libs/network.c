#include <sys/socket.h>
#include <netinet/in.h> 
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

char ** data;
struct pollfd* connectionfd;
struct pollfd* clientsfd;
int nclientsfd;

void initialize_network(){
	connectionfd = (struct pollfd *) malloc(sizeof(struct pollfd));
}

int read_data(){
	// printf("looking for data\n");
	int conn_ready = 0;
	char buffer[1024] = {0};
	if((conn_ready = poll(clientsfd,nclientsfd,0)) < 0){
		return 1;
	}
	else if(conn_ready > 0){
		for (int i = 0; i < nclientsfd; i++)
		{
			if (clientsfd[i].revents)
			{
				read(clientsfd[i].fd,buffer,1024);
				printf(buffer);
				fflush(stdout);
			}
			
		}
		
	}
	return 0;
}

int write_data(int sockfd,char * data){
	printf("writing data\n");
	send(sockfd,data,strlen(data),0);
	return 0;
}

int accept_connections(){
	// printf("looking for connections\n");
	int new_client_socket;
	struct sockaddr_in client_address; 
	int addrlen = sizeof(client_address); 
	int conn_ready = 0;
	if((conn_ready = poll(connectionfd,1,0)) < 0){
		return 1;
	}
	else if(conn_ready > 0){
		if ((new_client_socket = accept(connectionfd->fd, (struct sockaddr *)&client_address,(socklen_t*)&addrlen))<0) 
    	{
			return 2;
    	} 

		clientsfd = (struct pollfd *) realloc(clientsfd , (++nclientsfd) * sizeof(struct pollfd));
		if(clientsfd == NULL){
			return 3;
		}

		clientsfd[nclientsfd-1].fd = new_client_socket; 
		clientsfd[nclientsfd-1].events = POLLIN; 
		
	}
	return 0;
}

/**
 * Initialize a socket for tcp comunication
 * Bind the socket to localhost and port port
 * Listen on port with queue length of queue_len
 * Descriptor put in sockfd
 * returns 0 on success
 */
int initialize_socket(int port,int queue_len, int * sockfd){
	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
		printf("\n Socket creation error \n");
		return 1;
    }
	struct sockaddr_in address;
	address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons( port );
	if (bind(*sockfd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
		printf("\n Socket binding error \n");
		return 2; 
    }    

	if (listen(*sockfd, queue_len) < 0) 
    { 
		printf("\n Socket listening error \n");
		return 3; 
    } 
	connectionfd -> fd = *sockfd;
	connectionfd -> events = POLLIN;
	return 0;
}

int connect_to_server(int port, char* addr, int * sockfd){
	printf("connecting to server\n");
	struct sockaddr_in serv_addr; 
	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return 1; 
    } 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_addr.s_addr = inet_addr(addr);
    serv_addr.sin_port = htons(port); 
	if (connect(*sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return 2; 
    } 
	return 0;
}



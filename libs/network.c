#include <sys/socket.h>
#include <netinet/in.h> 
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "network.h"


#define QUEUE_LENGTH 100
#define KEYBOARD 0

char ** data;
struct pollfd* connectionfd;
struct pollfd* clientsfd;
int nclientsfd;
int conn_type;

int initialize_network(int type,int port,char* addr){
	conn_type = type;
	switch (conn_type)
	{
	case SERVER:
		nclientsfd = 0;
		connectionfd = (struct pollfd *) malloc(sizeof(struct pollfd));
		if(initialize_socket(port,QUEUE_LENGTH,&connectionfd->fd) != 0)
			return 1;
		break;
	case CLIENT:
		nclientsfd = 2;
		clientsfd = (struct pollfd *) malloc(nclientsfd * sizeof(struct pollfd));
		clientsfd[KEYBOARD].fd = stdin->_fileno;
		clientsfd[KEYBOARD].events = POLLIN;
		if(connect_to_server(port,addr,&clientsfd[1].fd) != 0)
			return 2;
		clientsfd[1].events = POLLIN;
		break;
	}
	return 0;
	
	
}

int initialize_socket(int port,int queue_len, int * sockfd){
	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
		// printf("\n Socket creation error \n");
		return 1;
    }
	struct sockaddr_in address;
	address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons( port );
	if (bind(*sockfd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
		// printf("\n Socket binding error \n");
		return 2; 
    }    

	if (listen(*sockfd, queue_len) < 0) 
    { 
		// printf("\n Socket listening error \n");
		return 3; 
    } 
	connectionfd -> fd = *sockfd;
	connectionfd -> events = POLLIN;
	return 0;
}

int accept_connections(){
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
		int space_found = 0;
		for(int i = 1 ; i <= nclientsfd ; i++){
			if(clientsfd[i].fd < 0){
				space_found = 1;
				clientsfd[i].fd = new_client_socket;
				clientsfd[nclientsfd-1].events = POLLIN; 
			}
		}
		if(!space_found){
			clientsfd = (struct pollfd *) realloc(clientsfd , (++nclientsfd) * sizeof(struct pollfd));
			if(clientsfd == NULL){
				return 3;
			}
			clientsfd[nclientsfd-1].fd = new_client_socket; 
			clientsfd[nclientsfd-1].events = POLLIN;
		} 
		
	}
	return 0;
}

int connect_to_server(int port, char* addr, int * sockfd){
	// printf("connecting to server\n");
	struct sockaddr_in serv_addr; 
	if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        // printf("\n Socket creation error \n"); 
        return 1; 
    } 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_addr.s_addr = inet_addr(addr);
    serv_addr.sin_port = htons(port); 
	if (connect(*sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        // printf("\nConnection Failed \n"); 
        return 2; 
    } 
	return 0;
}


int read_data(char * buffer, int bufflen,int * fd){

	int conn_ready = 0;
	if((conn_ready = poll(clientsfd,nclientsfd,0)) < 0){
		return -1;
	}
	else if(conn_ready > 0){
		for (int i = 0; i < nclientsfd; i++)
		{
			if (clientsfd[i].revents)
			{
				*fd = clientsfd[i].fd;
				int rd = 0;
				if((rd = read(clientsfd[i].fd,buffer,bufflen)) <= 0){
					clientsfd[i].fd = -1;
					clientsfd[i].revents = 0;
					// printf("client disconnected\n");
					return 0;
				}
				return rd;
				// printf(buffer);
				// fflush(stdout);
			}
			
		}
		
	}
	*fd = -1;
	return 0;
}

int write_data(int sockfd,const char const* data){
	// printf("writing data\n");
	if(conn_type == CLIENT)
		sockfd = clientsfd[1].fd;
	send(sockfd,data,strlen(data),0);
	return 0;
}

void closeConnections(){
	if (conn_type == CLIENT){
		close(clientsfd[1].fd);
		clientsfd[0].fd = 0;
	}
	else{
		for(int i = 0 ; i < nclientsfd ; i++)
			close(clientsfd[i].fd);
		close(connectionfd->fd);
	}
	free(clientsfd);
	free(connectionfd);
}





#if !defined(NETWORK_H_INCLUDED)
#define NETWORK_H_INCLUDED
void initialize_network();
int read_data();
int write_data(int sockfd,char * data);
int accept_connections();
int initialize_socket(int port,int queue_len, int * sockfd);
int connect_to_server(int port, char* addr, int * sockfd);
#endif // NETWORK_H_INCLUDED

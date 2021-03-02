#if !defined(NETWORK_H_INCLUDED)
#define NETWORK_H_INCLUDED
#define SERVER 0
#define CLIENT 1
#define PORT 8080
#define ADDR "127.0.0.1"
#define KEYBOARDIN stdin->_fileno
int initialize_network(int type,int port,char* addr);
int read_data(char * buffer, int bufflen,int * fd);
int write_data(int sockfd,const char const* data);
int accept_connections();
int initialize_socket(int port,int queue_len, int * sockfd);
int connect_to_server(int port, char* addr, int * sockfd);
void closeConnections();
#endif // NETWORK_H_INCLUDED

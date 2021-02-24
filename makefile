server: server/server.x
	gcc -Wall -o server/server.x server/main.c libs/network.c
client: client/client.x
	gcc -Wall -o client/client.x client/main.c libs/network.c
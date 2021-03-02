server: server/main.c libs/network.c
	gcc -Wall -o server/server.x server/main.c libs/network.c
client: client/main.c libs/network.c libs/logic.c client/UI.c
	gcc -Wall -o client/client.x client/main.c libs/network.c libs/logic.c client/UI.c
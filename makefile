server: server/main.c libs/network.c
	gcc -Wall -o server/server.x server/main.c libs/network.c
client: client/main.c libs/network.c libs/logic.c client/UI.c libs/state.c libs/messages.c
	gcc -Wall -o client/client.x client/main.c libs/network.c libs/logic.c client/UI.c libs/state.c libs/messages.c
client_debug :client/main.c libs/network.c libs/logic.c client/UI.c libs/state.c libs
	gcc -Wall -g -o client/client.x client/main.c libs/network.c libs/logic.c client/UI.c libs/state.c
	gdb client/client.x
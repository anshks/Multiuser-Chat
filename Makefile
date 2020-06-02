all: finalServer.c finalClient.c
	gcc finalServer.c -pthread
	gcc finalClient.c -o client -pthread

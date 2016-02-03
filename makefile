all: Server Client

Server: 
	gcc -W -Wall -pthread -o Server Server.c Utilities.c
Client: 
	gcc -W -Wall -pthread -o Client Client.c Utilities.c

Clean:
	rm -rf Server Client
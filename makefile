all: Clean Server Client

Server: 
	gcc -W -Wall -pthread -ggdb -o Server Server.c Utilities.c
Client: 
	gcc -W -Wall -pthread -ggdb -o Client Client.c Utilities.c

Clean:
	rm -rf Server Client

runNormal: doClient Time

runHigh: high Time

runMedium: medium Time

runLow:	low Time

runMin: min Time

doClient:
	./Client warandpeace

high:
	./Client warandpeace 5

medium:
	./Client warandpeace 20

low:
	./Client warandpeace 100

min:
	./Client warandpeace 1000

Time:
	date
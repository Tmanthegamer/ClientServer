

/*
===============================================================================
SOURCE FILE:    Client.h
                    Header file

PROGRAM:        Client Server

FUNCTIONS:      void* OutputFunction(void *message)
                int DisplayMessage(const char* message)
                int PromptUserInput(char* input)
                int ReadInput(void)
                int ReadServerResponse(void)
                int TerminateClient(void)


DATE:           January 9, 2016

REVISIONS:

DESIGNGER:      Tyler Trepanier-Bracken

PROGRAMMER:     Tyler Trepanier-Bracken

NOTES:
Standard Notes go here.
===============================================================================
*/
#include "Server.h"

extern int errno;       // error NO.
int quit = 0;

int main(void)
{
    
    sa.sa_handler = sig_handler;
  	sigemptyset (&sa.sa_mask);
  	sa.sa_flags = 0;

    sigaction (SIGINT, &sa, &oldint);
  	sigaction (SIGTSTP, &sa, NULL);

    Server();

    // Restore normal action
    sigaction (SIGINT, &oldint, NULL);

    return 0;
}

int Server(void)
{

    if(OpenQueue() < 0)
        return 1;

    //Insert ProcessInput functional inside here.
    SearchForClients();

    RemoveQueue(msgQueue);
    /*
    Create ClientServer msg queue
    Assign Message-Type 1 to be the messages designated Client to Server
    Goto SEARCH FOR CLIENTS
    */
    return 0;
}

int SearchForClients(void)
{
    Mesg rcv;

    while (!quit){
        if(ReadMessage(msgQueue, &rcv, CLIENT_TO_SERVER) == 0)
        {
            pid_t child;
            child = fork();

            switch(child)
            {
            case -1:
                printf("Fatal error.\n");
                break;
            case 0: //child

                ProcessClient(&rcv, msgQueue);
                exit(1);
                break;

            default: //parent
                //No action to be taken for the parent
                break;
            }

        }

    }


    printf("Killed SearchForClients.\n");
    /*
    Check for any incoming messages from clients.
    Failure no new messages: RESTART SEARCH FOR CLIENTS
    Pass message to CREATE NEW CLIENT PROCESS.
    Goto CREATE NEW CLIENT PROCESS
    */
    return 0;
}

int ProcessClient(Mesg* msg, int queue)
{
    FILE* file;
    char name[BUFF];
    pid_t client;
    int priority;

    if(DesignatePriority(msg->mesg_data, name, &priority, &client) < 0)
    {
        printf("Fatal error, cannot read message.");
        return -1;
    }

    if((file = OpenFile(name)) == NULL)
    {
        msg->mesg_type = client;
        sprintf(msg->mesg_data, "Cannot open file: %s\n", name);
       if(SendMessage(queue, msg) < 0){
            perror("Unable to send message");
            return -1;
        }

        if(SendFinalMessage(queue, msg) < 0)
        {
            printf("Can't send final message.\n");
            return -1;
        }

        /*
        Failure: File cannot be opened for reading
            Send “Error: Cannot open file” message to SEND MESSAGE
            Goto SEND MESSAGE
        */
        return 0;
    }
    else
    {
        printf("Sending %s to client:%d\n", name, client);
        PacketizeData(file, queue, (long)client, priority);
    }

    /*
    Grab received message
    Get Client’s Priority and PID
    Goto OPEN REQUESTED FILE
    */

    return 0;
}



int DesignatePriority(const char* text,
                      char* name,
                      int* priority,
                      pid_t* client)
{

    int n = sscanf(text, "%s %d %d", name, priority, client);
    
    if (n == 3)
    {
        return 1;
    }

    return -1;
}

int PacketizeData(FILE* fp,
                  const int queue,
                  const long msg_type,
                  const int priority)
{
    Mesg snd;
    int m_size = MAXMESSAGEDATA;

    if (priority < 0)
        m_size = MAXMESSAGEDATA;
    else if (priority > 1000)
        m_size = MAXMESSAGEDATA / 1000;
    else
        m_size = MAXMESSAGEDATA / priority;

    char buf[m_size];

    snd.mesg_type = msg_type;
    // Priority is organized by dividing the message by its priority number.

    //don't use fgets, copy each char individually until the limit is filled.
    while(fgets(buf, m_size, fp) != NULL)
    {
        strcpy(snd.mesg_data, buf);

        if(SendMessage(queue, &snd) < 0){
            printf("Error in PacketizeData, SendMessage fail\n");
        }
    }


    if(SendFinalMessage(queue, &snd) < 0){
        printf("Error in ending msg, SendMessage fail\n");
    }
    printf("Sending to %ld complete...\n", msg_type);
    fclose(fp);

    return 0;
}

void Cleanup(void)
{

}

void ServerHelp(void)
{
    printf("\n==================================================\n");
    printf("               SERVER PROGRAM USAGE                \n");
    printf("==================================================\n\n");
    printf("This application requires no user input to run   \n");
    printf("however you can close the server at anytime by   \n");
    printf("pressing the 'q' key on your keyboard or ctrl-c. \n");
    printf("=================================================\n");
}

void* Input(void* unused)
{
	char message;

	printf("You can press 'h' at anytime for this program's usage.");
	while((message = fgetc(stdin)) != EOF)
	{
		if(message == 'h')
			ServerHelp();
		else if(message == 'q')
		{
			quit = 1;
			break;
		}
			
	}
    return 0;
}

/* Simple signal handler */
void sig_handler(int sig)
{
    if(sig){

    }
    RemoveQueue(msgQueue);
    fflush(stdout);
    /*
    ADD IN MORE CLEAN UP DUTIES HERE.
    */

    exit(1);
}
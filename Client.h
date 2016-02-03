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

#include <pthread.h>
#include "Utilities.h"

int done, ready;

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 28, 2016

REVISIONS:      January 30, 2016    (Tyler Trepanier-Bracken)
                    Moved existing code for sending test messages into Server
                    function.
                Febuary 1, 2016     (Tyler Trepanier-Bracken)
                    Create the functionality to split the server and client
                    components via command-line arguments.
                Febuary 2, 2016     (Tyler Trepanier-Bracken)
                    Split this function into separate files: Client and 
                    Server.

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int main(void)

RETURNS:        -Returns 1 on improper program exit.
                -Returns 0 on normal program termination.

NOTES:
Main entry point into the program. Divides program functionality based on the 
command-line arguments into client and server functionality. 
===============================================================================
*/
int main(void);

/*
===============================================================================
FUNCTION:       Client

DATE:           January 30, 2016

REVISIONS:      Febuary 1, 2016 (Tyler Trepanier-Bracken)
                    Implemented threads to read from a server.

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int Client(void)

PARAMETERS:     void

RETURNS:        -Returns 1 if the Client was unable to open a message queue.
                -Returns 0 on proper program termination

NOTES:
Separate functionality from the Server side, the purpose of this function 
is to allow user requests for files. First it opens the message queue (if it 
cannot open the message queue, this will exit the application). The request 
will be padded with the PID of the current process and the optional priority. 

Afterwards the server will respond with the contents of the file inside of a
series of messages. If the server cannot open the file, the server will 
respond with an error.

The program will continually asked for user input until user enters "quit" 
or ctrl-c has been hit.
===============================================================================
*/
int Client(void);

/*
===============================================================================
FUNCTION:       Read Server Response 

DATE:           Febuary 1, 2016

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      void* ReadServerResponse(void *queue)

PARAMETERS:     void *queue
                    Pointer to an integer which is the message queue that
                    the server reads on.

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
This is a thread function that contiuously reads the message queue for any 
messages in the queue that are meant for this Client.
===============================================================================
*/
void* ReadServerResponse(void *queue);

/*
===============================================================================
FUNCTION:       Prompt User Input

DATE:           January 31, 2016

REVISIONS:      Febuary 2, 2016 (Tyler Trepanier-Bracken)
                    Implemented help messages and normal termination via 
                    requesting a program quit.

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int PromptUserInput(char* input)

PARAMETERS:     char *input
                    The buffer which will be written onto. This will hold
                    the client's pid, priority and the filename when the user
                    succesfully inputs the required information.

RETURNS:        -Returns -1 on a quit request
                -Returns 0 on succesful user input
                -Returns 1 on help message requested.

NOTES:
This function grabs filenames from stdin (user input). In addition, the user
is prompted to add in priority or even a client's process ID.

When first prompted with filename request, the user has the ability to quit
the application by entering "quit". The user can also request on how to use
the application by entering "help".
===============================================================================
*/
int PromptUserInput(char* input);

/*
===============================================================================
FUNCTION:       Create Read Thread 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int CreateReadThread(void)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int CreateReadThread(void);

/*
===============================================================================
FUNCTION:       Client Help

DATE:           Febuary 2, 2016            

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      void ClientHelp(void);

PARAMETERS:     void

NOTES:
Displays a help message to standard output that displays how to use this
Client application.
===============================================================================
*/
void ClientHelp(void);
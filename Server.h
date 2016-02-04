/* 
===============================================================================
SOURCE FILE:    Client.h 
                    Header file 

PROGRAM:        Client

FUNCTIONS:      int main(void)
                int Client(void)
                int PromptUserInput(char* input)
                int CreateReadThread(void)
                void* ReadServerResponse(void *queue)
                void sig_handler(int sig)


DATE:           January 30, 2016

REVISIONS:      January 31, 2016        (Tyler Trepanier-Bracken)
                    Splitting the old "ClientServer" functionality into
                    the Client files and the Server files. In addition, 
                    worked on Prompt-User Input for bug fixing and formatting
                    the desired message.
                
                Febuary 2, 2016         (Tyler Trepanier-Bracken)
                    Both Client and Server files have been split from each
                    other and any shared functionality has been included
                    inside of the Utilities files. NOTE: Both Server and
                    Client has their own definitions of the sig_handler
                    with their own implementations.

DESIGNGER:      Tyler Trepanier-Bracken

PROGRAMMER:     Tyler Trepanier-Bracken

NOTES:
Client is a stand-alone program which allows the user to ask for files to read
and displayed onto the stdout of this process. This client may have multiple
instances at once and they work in unison with only one Server. 

Each individual Client requests the user to enter in the filename and, if
they so please, the priority and another client's process id. This request
gets bundled into a message which gets sent to the server where the server
processes this file opening request. Afterwards, the server responds which
either an error message or the contents of the file.

This is the main file that holds all of the unique functionality of the Client
program. There are some shared functionality with the Server that is defined
inside of the Utilities files.
===============================================================================
*/

#include "Utilities.h"

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 28, 2016

REVISIONS:      January 30, 2016    (Tyler Trepanier-Bracken)
                    Moved existing code for sending test messages into Server
                    function.
                Febuary 30, 2016     (Tyler Trepanier-Bracken)
                    Create the functionality to split the server and client
                    components via command-line arguments.

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int main(void)

PARAMETERS:     void

RETURNS:        -Returns 1 on improper program exit.
                -Returns 0 on normal program termination.

NOTES:
Main entry point into the program. Divides program functionality based on the 
command-line arguments into client and server functionality. 

Server will start when the Program is called with the 's' character.

Client will start when the Program is called with the 'c' character.
===============================================================================
*/
int main(void);

/*
===============================================================================
FUNCTION:       Server 

DATE:           January 28, 2016

REVISIONS:      Febuary 1, 2016 (Tyler Trepanier)
                    Removed deprecated message queue tests and debug 
                    statements.

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int Server(void)

PARAMETERS:     void

RETURNS:        -Returns 1 if the Server was unable to open a message queue.
                -Returns 0 on proper program termination

NOTES:
Separate functionality from the Client side, the purpose of this function is
to service all incoming client messages. First it opens the message queue (if
it cannot open the message queue, this will exit the application) and searches
for all incoming clients. 

Upon user request, the message queue will terminate.
===============================================================================
*/
int Server(void);

/*
===============================================================================
FUNCTION:       Process Client 

DATE:           January 30, 2016

REVISIONS:      Feb 1, 2016 (Tyler Trepanier-Bracken)
                    Implemented final message to let the client know that the
                    sending for a specific file is finished. Previously,
                    there was no final message and client was stuck reading
                    forever.

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int main (char *process)

PARAMETERS:     Mesg* msg, 
                int queue

RETURNS:        -Returns -1 on failure to Open the client requested file.       
                -Returns 0 if the file was opened and sent successfully.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int ProcessClient(Mesg* msg, int queue);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int PacketizeData(FILE* fp,
                  const int queue,
                  const long msg_type,
                  const int priority);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
int DesignatePriority(const char* text,
                      char* name,
                      int* priority,
                      pid_t* client);

/*
===============================================================================
FUNCTION:       Main 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int main (char *process)

PARAMETERS:     char *process: 
                    the name of the process to be validated. 

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Standard Notes go here. 
===============================================================================
*/
void Cleanup(void);

/*
===============================================================================
FUNCTION:       Search For Clients 

DATE:           January 30, 2016            

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int SearchForClients(void)

PARAMETERS:     void

RETURNS:        -Returns 1 on improper termination.
                -Returns 0 when the user gracefully terminates the reading.

NOTES:
Searches for multiple clients and assigns each client a separate process.
===============================================================================
*/
int SearchForClients(void);

/*
===============================================================================
FUNCTION:       Server Help

DATE:           Febuary 2, 2016            

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      void ServerHelp(void);

PARAMETERS:     void

NOTES:
Displays a help message to standard output that displays how to use this
Server application.
===============================================================================
*/
void ServerHelp(void);

/*
===============================================================================
FUNCTION:       Search For Clients 

DATE:           January 30, 2016            

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int SearchForClients(void)

PARAMETERS:     void

RETURNS:        -Returns 1 on improper termination.
                -Returns 0 when the user gracefully terminates the reading.

NOTES:
Searches for multiple clients and assigns each client a separate process.
===============================================================================
*/
void* Input(void* unused);

/*
===============================================================================
FUNCTION:       Search For Clients 

DATE:           January 30, 2016            

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken

INTERFACE:      int SearchForClients(void)

PARAMETERS:     void

RETURNS:        -Returns 1 on improper termination.
                -Returns 0 when the user gracefully terminates the reading.

NOTES:
Searches for multiple clients and assigns each client a separate process.
===============================================================================
*/
int CreateInputThread(void);
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

INTERFACE:      int main(int argc,
                         char **argv)

PARAMETERS:     int argc,
                    Number of command-line arguments. Used to verify the
                    correctness of program calling.
                char **argv
                    Command-line arguments. Only second argument is taken
                    into consideration for this program.
RETURNS:        -Returns 1 on improper program exit.
                -Returns 0 on normal program termination.

NOTES:
Main entry point into the program. Divides program functionality based on the 
command-line arguments into client and server functionality. 

Server will start when the Program is called with the 's' character.

Client will start when the Program is called with the 'c' character.
===============================================================================
*/
int main(int argc,char **argv);

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
/* 
===============================================================================
SOURCE FILE:    Server.h 
                    Header file for the Server

PROGRAM:        Server

FUNCTIONS:      int main(void)
                int Server(void)
                int SearchForClients(void)
                int ProcessClient(Mesg* msg, int queue)
                int DesignatePriority(const char* text,
                      char* name,
                      int* priority,
                      pid_t* client)
                int PacketizeData(FILE* fp,
                      const int queue,
                      const long msg_type,
                      const int priority)
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
Server is a stand-alone program which allows any incoming Clients to request 
files to be read and its information sent to the client. There should be only 
one instance of Server as both Servers can cause collisions with each other.

The Server uses the client's filename, mentioned in the mesg_data, to open
a new file for reading. Upon success, it will disperse the contents of the
file to the Client. Failure will simply send a small message to the Client
indicating failure to open the file.

In addition, the Client can specify their priority level (defaults to max),
which will affect the speed of transmission between the Client and the Server.

This is the main file that holds all of the unique functionality of the Server
program. There are some shared functionality with the Client that is defined
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

RETURNS:        Returns 0 on normal program termination.

NOTES:
Main entry point into the program. Simply sets the sig_actions structure to
catch any sig_int and uses the sig_handler function to deal with any issues.
Afterwards, the Server function is called to run the program.
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

RETURNS:        -Returns -1 on failure to Open the client requested file or
                    it cannot send the final message.       
                -Returns 0 if the file was opened and sent successfully.

NOTES:
This is where the child process created by the Search for Client function ends
up. 

This function parses the message data, attempts to open the file. If the
file cannot open properly, it will send a message to the client indicated file
open failure. Otherwise, the file's contents will be sent to the Client using
the Packetize Data function.
===============================================================================
*/
int ProcessClient(Mesg* msg, int queue);

/*
===============================================================================
FUNCTION:       PacketizeData 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int PacketizeData(FILE* fp,
                  const int queue,
                  const long msg_type,
                  const int priority);

PARAMETERS:     FILE* fp,
                    File pointer to a previously opened file for reading.
                const int queue,
                    The message queue on which the server will write
                    messages to.
                const long msg_type,
                    The message type of the client who will receive the
                    series of messages.
                const int priority);
                    How urgent the clients wishes to receive the data. It is
                    a number in between 1-1000.                    

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
This function depends on a successfully opened file. This function performs
the task of separting the contents of the file into packet sized blocks which
will then be sent to the client using the given queue and message type. The 
sending will continue until the end-of-file has been reached in the file.

After a successful read or a ctrl-c is catched, the server will send the final
message indicating that the reading is finished.
===============================================================================
*/
int PacketizeData(FILE* fp,
                  const int queue,
                  const long msg_type,
                  const int priority);

/*
===============================================================================
FUNCTION:       Designate Priority 

DATE:           January 9, 2016

REVISIONS:      (Date and Description)

DESIGNER:       Tyler Trepanier-Bracken

PROGRAMMER(S):  Tyler Trepanier-Bracken
                Harvey Dent

INTERFACE:      int DesignatePriority(const char* text,
                      char* name,
                      int* priority,
                      pid_t* client)

PARAMETERS:     const char* text,
                    File pointer to a previously opened file for reading.
                char* name,
                    The message queue on which the server will write
                    messages to. This is the filename which is the first
                    part of the message that is parsed.
                int priority);
                    How urgent the clients wishes to receive the data. This
                    field is filled using the text's data.
                pid_t* long client,
                    The message type of the client who will receive the
                    series of messages. This field is filled using the text's 
                    data.

RETURNS:        -Returns the PID of process specified if the process
                exists.          
                -Returns 0 if the process was not found in the process table.

NOTES:
Simple wrapper function that parses the text received by the client and
extracts the name of the file, the designated priority and the client's PID
(which will become the message type).
===============================================================================
*/
int DesignatePriority(const char* text,
                      char* name,
                      int* priority,
                      pid_t* client);

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

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
#include "ClientServer.h"

int done;
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
void* ReadServerResponse(void *queue);

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
int PromptUserInput(char* input);

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
int ReadInput(void);

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
int CreateReadThread();
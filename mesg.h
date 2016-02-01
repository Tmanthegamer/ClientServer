/*
===============================================================================
SOURCE FILE:    mesg.h
                    Defines various elements necessary for a message to be
                    passed within the linux message queue. Builds on top of the
                    standard generic linux message structure by adding in the
                    ability to read in variable sized messages by defining the
                    maximum size of message. Includes the length (in bytes) of
                    the message data.

PROGRAM:        Client Server

DATE:           January 21, 2016

REVISIONS:		January 30, 2016
					Changed the position of the mesg_len inside of the Mesg
					definition, the mesg_type was being inserted inside of
					there inside of the message queue.

DESIGNGER:      Tyler Trepanier-Bracken

PROGRAMMER:     Tyler Trepanier-Bracken

NOTES:
Standard Notes go here.
===============================================================================
*/

 /* don't want sizeof(Mesg) > 4096 */
#define MAXMESSAGEDATA 	(4096-16)

/* length of mesg_len and mesg_type */
#define MESGHDRSIZE 	(sizeof(Mesg) - MAXMESGDATA)

/*
Mesg structure used to transfer messages between the client processes and server process
*/
typedef struct
{
	long mesg_type; /* message type */
	size_t mesg_len; /* #bytes in mesg_data */
	char mesg_data[MAXMESSAGEDATA];
}

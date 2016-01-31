#include <pthread.h>
#include "ClientServer.h"

void* OutputFunction(void *message);

int DisplayMessage(const char* message);

int PromptUserInput(char* input);

int ReadInput(void);

int ReadServerResponse(void);

int TerminateClient(void);

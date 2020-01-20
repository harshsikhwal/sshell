#include<string.h>
#include<stdio.h>
#include<stdlib.h>

// ARROW COUNTERS

int UP_ARROW_COUNT = 0;
int DOWN_ARROW_COUNT = 0;
int LEFT_ARROW_COUNT = 0;
int RIGHT_ARROW_COUNT = 0;

struct termios original = {0};

struct archiveCommandList
{
    char _value[256];
    struct archiveCommandList *LLINK;
    struct archiveCommandList *RLINK;
};

typedef struct archiveCommandList archivedCommands;

archivedCommands* startACL = NULL;
archivedCommands* iterACL = NULL;


// Terminal Functions
void initTerminal()
{
    printf("\nInitializing Terminal....\n");
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    if (tcgetattr(0, &original) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
    printf("\nDone Initializing Terminal\n\n");
}

void resetTerminal()
{
    printf("\nResetting Terminal....\n");
    if (tcsetattr(0, TCSANOW, &original) < 0)
            perror("tcsetattr ICANON");
    printf("\nReset Terminal Complete\n");
}


// Archived Command List Functions
void initializeACL()
{
    startACL = (archivedCommands *)malloc(sizeof(archivedCommands *));
    iterACL = (archivedCommands *)malloc(sizeof(archivedCommands *));
}


void add2ACL(char *value)
{
    archivedCommands* newArchivedCommands = (archivedCommands *)malloc(sizeof(archivedCommands *));
    archivedCommands* iter;
    //printf("\nValue to Copy: %s\nSize: %d\n", value, strlen(value));
    if((strcmp(value, "\n") == 0) || (strcmp(value, "history") == 0) || (strcmp(value, "\0") == 0))
        return;
    //printf("Adding value\n");
    strncpy(newArchivedCommands->_value, value, strlen(value));
    // Iterate the ACL

    if(startACL->RLINK == NULL)
    {
        startACL->RLINK = newArchivedCommands;
        newArchivedCommands->LLINK = startACL;
        newArchivedCommands->RLINK = NULL;
        startACL->LLINK = NULL;
    }
    else
    {
        iter = startACL;
        while(NULL != iter->RLINK)
        {
            iter = iter->RLINK;
        }
        iter->RLINK = newArchivedCommands;
        newArchivedCommands->LLINK = iter;
        newArchivedCommands->RLINK = NULL;
    }

    //free(newArchivedCommands);
    //free(iter);
}

void printACL()
{
    archivedCommands* iter;
    if(NULL == startACL->RLINK)
        return;
    else
    {
        iter = startACL->RLINK;
        while(NULL != iter->RLINK)
        {
            printf("%s\n", iter->_value);
            iter = iter->RLINK;
        }
        printf("%s\n", iter->_value);
    }
    free(iter);
}

char* getACLIteratorString()
{
    if(iterACL != startACL)
    {
        return iterACL->_value;
    }
    else
    {
        return "\0";
    }
}

void moveACLIterator()
{
    if(NULL == iterACL->RLINK)
    {
        // DO NOTHING
    }
    else
    {
        iterACL =iterACL->RLINK;
    }
}

void resetACLIterator()
{
    iterACL = NULL;
    iterACL = startACL;
}

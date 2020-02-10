#include<string.h>
#include<stdio.h>
#include<stdlib.h>

// Current working Directory
char cwd[256];

// ARROW COUNTERS

int UP_ARROW_COUNT = 0;
int DOWN_ARROW_COUNT = 0;
int LEFT_ARROW_COUNT = 0;
int RIGHT_ARROW_COUNT = 0;

struct termios original = {0};

struct archive_command_struct
{
    char _value[256];
    struct archive_command_struct *LLINK;
    struct archive_command_struct *RLINK;
};

typedef struct archive_command_struct archived_statements;

archived_statements *asl_head = NULL;
archived_statements *asl_iterator = NULL;


// Terminal Functions
void terminal_init()
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

void terminal_reset()
{
    printf("\nResetting Terminal....\n");
    if (tcsetattr(0, TCSANOW, &original) < 0)
            perror("tcsetattr ICANON");
    printf("\nReset Terminal Complete\n");
}


// Archived Command List Functions
void asl_init()
{
    asl_head = (archived_statements *)malloc(sizeof(archived_statements *));
    asl_iterator = (archived_statements *)malloc(sizeof(archived_statements *));
}


void asl_add(char *value)
{
    archived_statements *newarchived_statements = (archived_statements *)malloc(sizeof(archived_statements *));
    archived_statements *iter;
    //printf("\nValue to Copy: %s\nSize: %d\n", value, strlen(value));
    if((strcmp(value, "\n") == 0) || (strcmp(value, "history") == 0) || (strcmp(value, "\0") == 0))
        return;
    //printf("Adding value\n");
    strncpy(newarchived_statements->_value, value, strlen(value));
    // Iterate the asl
    if(asl_head->RLINK == NULL)
    {
        asl_head->RLINK = newarchived_statements;
        newarchived_statements->LLINK = asl_head;
        newarchived_statements->RLINK = NULL;
        asl_head->LLINK = NULL;
    }
    else
    {
        iter = asl_head;
        while(NULL != iter->RLINK)
        {
            iter = iter->RLINK;
        }
        iter->RLINK = newarchived_statements;
        newarchived_statements->LLINK = iter;
        newarchived_statements->RLINK = NULL;
    }
}

void asl_print()
{
    archived_statements *iter;
    if(NULL == asl_head->RLINK)
        return;
    else
    {
        iter = asl_head->RLINK;
        while(NULL != iter->RLINK)
        {
            printf("%s\n", iter->_value);
            iter = iter->RLINK;
        }
        printf("%s\n", iter->_value);
    }
    free(iter);
}

char* asl_get_iter_string()
{
    //printf("\naslIteratorString: %s\n", asl_iterator->_value);
    if(asl_iterator == asl_head)
    {
        return "\0";
    }
    else
    {
        return asl_iterator->_value;
    }
}

void asl_iterator_move()
{
    if(NULL == asl_iterator->RLINK)
    {
        // DO NOTHING
    }
    else
    {
        asl_iterator =asl_iterator->RLINK;
    }
}

void asl_iterator_reset()
{
    asl_iterator = NULL;
    asl_iterator = (archived_statements *)malloc(sizeof(archived_statements *));
    asl_iterator = asl_head;
}

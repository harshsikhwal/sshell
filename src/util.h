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

struct command_data_struct
{
    char _command[256];
    char _flags[128][128];
    char _values[128][256];
    unsigned int _flags_count;
    unsigned int _values_count;
};

typedef struct command_data_struct command_data;

struct archive_statement_struct
{
    char _value[4096];
    struct archive_statement_struct *_LLINK;
    struct archive_statement_struct *_RLINK;
};

typedef struct archive_statement_struct archived_statements;

archived_statements *asl_head = NULL;
archived_statements *asl_iterator = NULL;


int console_print(char *str)
{
    printf("%s\n", str);
}

// Terminal Functions
void terminal_init()
{
    console_print("\nInitializing Terminal....");
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
    console_print("Done Initializing Terminal\n");
}

void terminal_reset()
{
    console_print("\nResetting Terminal....\n");
    if (tcsetattr(0, TCSANOW, &original) < 0)
            perror("tcsetattr ICANON");
    console_print("\nReset Terminal Complete\n");
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
    if(asl_head->_RLINK == NULL)
    {
        asl_head->_RLINK = newarchived_statements;
        newarchived_statements->_LLINK = asl_head;
        newarchived_statements->_RLINK = NULL;
        asl_head->_LLINK = NULL;
    }
    else
    {
        iter = asl_head;
        while(NULL != iter->_RLINK)
        {
            iter = iter->_RLINK;
        }
        iter->_RLINK = newarchived_statements;
        newarchived_statements->_LLINK = iter;
        newarchived_statements->_RLINK = NULL;
    }
}

void asl_print()
{
    archived_statements *iter;
    if(NULL == asl_head->_RLINK)
        return;
    else
    {
        iter = asl_head->_RLINK;
        while(NULL != iter->_RLINK)
        {
            console_print(iter->_value);
            iter = iter->_RLINK;
        }
        console_print(iter->_value);
    }
    free(iter);
}

char* asl_get_iter_string()
{
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
    if(NULL == asl_iterator->_RLINK)
    {
        // DO NOTHING
    }
    else
    {
        asl_iterator = asl_iterator->_RLINK;
    }
}

void asl_iterator_reset()
{
    asl_iterator = NULL;
    asl_iterator = (archived_statements *)malloc(sizeof(archived_statements *));
    asl_iterator = asl_head;
}

void asl_delete()
{
    archived_statements *current, *next;
    // asl empty
    if(NULL == asl_head->_RLINK)
        return;
    else
    {
        current = asl_head->_RLINK;
        while(NULL != current)
        {
            next = current->_RLINK;
            free(current);
            current = next;
        }
    }
    free(current);
    asl_init();
    asl_iterator_reset();
}

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

typedef struct archive_command_struct archived_commands;

archived_commands *acl_head = NULL;
archived_commands *acl_iterator = NULL;


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
void acl_init()
{
    acl_head = (archived_commands *)malloc(sizeof(archived_commands *));
    acl_iterator = (archived_commands *)malloc(sizeof(archived_commands *));
}


void acl_add(char *value)
{
    archived_commands *newarchived_commands = (archived_commands *)malloc(sizeof(archived_commands *));
    archived_commands *iter;
    //printf("\nValue to Copy: %s\nSize: %d\n", value, strlen(value));
    if((strcmp(value, "\n") == 0) || (strcmp(value, "history") == 0) || (strcmp(value, "\0") == 0))
        return;
    //printf("Adding value\n");
    strncpy(newarchived_commands->_value, value, strlen(value));
    // Iterate the ACL
    if(acl_head->RLINK == NULL)
    {
        acl_head->RLINK = newarchived_commands;
        newarchived_commands->LLINK = acl_head;
        newarchived_commands->RLINK = NULL;
        acl_head->LLINK = NULL;
    }
    else
    {
        iter = acl_head;
        while(NULL != iter->RLINK)
        {
            iter = iter->RLINK;
        }
        iter->RLINK = newarchived_commands;
        newarchived_commands->LLINK = iter;
        newarchived_commands->RLINK = NULL;
    }
}

void acl_print()
{
    archived_commands *iter;
    if(NULL == acl_head->RLINK)
        return;
    else
    {
        iter = acl_head->RLINK;
        while(NULL != iter->RLINK)
        {
            printf("%s\n", iter->_value);
            iter = iter->RLINK;
        }
        printf("%s\n", iter->_value);
    }
    free(iter);
}

char* acl_get_iter_string()
{
    //printf("\nACLIteratorString: %s\n", acl_iterator->_value);
    if(acl_iterator == acl_head)
    {
        return "\0";
    }
    else
    {
        return acl_iterator->_value;
    }
}

void acl_iterator_move()
{
    if(NULL == acl_iterator->RLINK)
    {
        // DO NOTHING
    }
    else
    {
        acl_iterator =acl_iterator->RLINK;
    }
}

void acl_iterator_reset()
{
    acl_iterator = NULL;
    acl_iterator = (archived_commands *)malloc(sizeof(archived_commands *));
    acl_iterator = acl_head;
}

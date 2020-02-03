#include<string.h>
#include<stdio.h>
#include<stdlib.h>

struct command_entry_struct
{
    char _command[256];
    char _flags[128][1];
    struct command_entry_struct *_NEXT;
    //struct archiveCommandList *RLINK;
};

typedef struct command_entry_struct command_entry;

command_entry *command_registry_head = NULL;

command_entry *create_command_entry(char *command_value, char **flag_value)
{
    command_entry *new_entry = (command_entry *)malloc(sizeof(command_entry *));
    strcpy(new_entry->_command, command_value);
    //TODO: Need to update the flags HERE
    new_entry->_NEXT = NULL;
    return new_entry;
}

void command_register_add(command_entry *new_entry)
{
    command_entry *iter;
    iter = command_registry_head;
    if(iter->_NEXT == NULL)
    {
        iter->_NEXT = new_entry;
    }
    else
    {
        while(iter->_NEXT != NULL)
        {
            iter = iter->_NEXT;
        }
        iter->_NEXT = new_entry;
    }
}

void command_register_init()
{
    command_entry *new_entry;
    command_registry_head = (command_entry *)malloc(sizeof(command_entry *));
    strcpy(command_registry_head->_command, "SIGNATURE_BEGIN");
    command_registry_head->_NEXT = NULL;

    // HOOK YOUR COMMAND HERE
    char **flags[128][1] = {{'c'}};
    new_entry = create_command_entry("history", flags);
    command_register_add(new_entry);

}

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include"commands/standard_commands.h"

struct command_entry_struct
{
    char _command[256];
    unsigned int _command_id;
    char _flags[128][128];
    int _flag_size;
    struct command_entry_struct *_NEXT;
    //struct archiveCommandList *RLINK;
};

typedef struct command_entry_struct command_entry;

command_entry *command_registry_head = NULL;

command_entry *create_command_entry(int command_id, char* command, int num_flags, ...)
{
    command_entry *new_entry = (command_entry *)malloc(sizeof(command_entry *));
    va_list argument_list;
    // Add the arguments in ap
    va_start(argument_list, num_flags);
    // First value will be command value
    //int i = 0;
    strcpy(new_entry->_command, command);//va_arg(argument_list, char *));
    new_entry->_command_id = command_id;
    new_entry->_NEXT = NULL;
    // Iterate the rest and add them to flags
    int i = 0;
    int array_count = 0;
    for( ; i < num_flags; i++ )
    {
        strcpy(new_entry->_flags[array_count++], va_arg(argument_list, char *));
    }
    new_entry->_flag_size = array_count;
    va_end(argument_list);
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

    // Command exit
    new_entry = create_command_entry(1, "exit", 0);
    command_register_add(new_entry);
    // Command history
    new_entry = create_command_entry(2, "history", 1, "c");
    command_register_add(new_entry);
    // Command clear
    new_entry = create_command_entry(3, "clear", 0);
    command_register_add(new_entry);
    // Command user, commented as it has some issues
    // new_entry = create_command_entry(4, "user", 0);
    // command_register_add(new_entry);
    // Command echo
    new_entry = create_command_entry(5, "echo", 0);
    command_register_add(new_entry);

}

int command_search(char* command_value)
{
    command_entry *iter;
    iter = command_registry_head;
    if(iter->_NEXT == NULL)
    {
        // Command registry empty
        return -1;
    }
    else
    {
        // Skip SIGNATURE_BEGIN
        iter = iter->_NEXT;
        while(1)
        {
            if( 0 == strcmp(iter->_command, command_value))
            {    // found, return command id
                return iter->_command_id;
            }
            iter = iter->_NEXT;
            if(NULL == iter)
                return 0;
        }
    }
}

int master_command_handler(int command_id, command_data* c_data, char* statement)
{
    switch(command_id)
    {
        case 1: // exit
                return command_exit(c_data, statement);

        case 2:  // history
                return command_history(c_data, statement);

        case 3: // clear
                return command_clear(c_data, statement);

        case 4: // clear
                return command_user(c_data, statement);

        case 5: // clear
                return command_echo(c_data, statement);

    }
}
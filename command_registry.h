#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

struct command_entry_struct
{
    char _command[256];
    char _flags[128][128];
    int _flag_size;
    struct command_entry_struct *_NEXT;
    //struct archiveCommandList *RLINK;
};

typedef struct command_entry_struct command_entry;

command_entry *command_registry_head = NULL;

command_entry *create_command_entry(int num_args, ...)
{
    command_entry *new_entry = (command_entry *)malloc(sizeof(command_entry *));
    va_list argument_list;
    // Add the arguments in ap
    va_start(argument_list, num_args);
    // First value will be command value
    int i = 0;
    strcpy(new_entry->_command, va_arg(argument_list, char *));
    new_entry->_NEXT = NULL;
    // Iterate the rest and add them to flags
    i = 1;
    int array_count = 0;
    for( ; i < num_args; i++ )
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
    new_entry = create_command_entry(2, "history", "c");
    command_register_add(new_entry);

    new_entry = create_command_entry(1, "exit");
    command_register_add(new_entry);

}

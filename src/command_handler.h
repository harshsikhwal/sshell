/*
 * MIT License
 *
 * Copyright (c) 2020 Harsh Sikhwal
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

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

int command_registry_printer();

void command_register_add(command_entry *new_entry)
{
    command_entry *iter = (command_entry *)malloc(sizeof(command_entry *));
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

command_entry* create_command_entry(int command_id, char* command, int num_flags, ...)
{
    command_entry *new_entry = (command_entry *)malloc(sizeof(command_entry *));
    // initialize_char_array(new_entry->_command, 256);
    strcpy(new_entry->_command, get_empty_array(256));
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
    //free(new_entry);
}

void command_register_init()
{
    command_entry *new_entry;
    command_registry_head = (command_entry *)malloc(sizeof(command_entry *));
    // memset(command_registry_head->_command, '\0', 256);
    strcpy(command_registry_head->_command, get_empty_array(256));
    strcpy(command_registry_head->_command, "SIGNATURE_BEGIN");
    command_registry_head->_NEXT = NULL;

    // HOOK YOUR COMMAND HERE

    // Command exit
    new_entry = create_command_entry(1, "exit", 0);
    command_register_add(new_entry);
    // Command history
    new_entry = create_command_entry(2, "history", 1, "c");
    command_register_add(new_entry);
    // Command cd
    new_entry = create_command_entry(3, "cd", 0);
    command_register_add(new_entry);
    // Command user, commented as it has some issues
    // new_entry = create_command_entry(4, "user", 0);
    // command_register_add(new_entry);
    // Command echo
    new_entry = create_command_entry(5, "echo", 0);
    command_register_add(new_entry);
    // Command mkdir
    new_entry = create_command_entry(6, "mkdir", 0);
    command_register_add(new_entry);
    // Command file
    new_entry = create_command_entry(7, "file", 0);
    command_register_add(new_entry);
    // Command help
    new_entry = create_command_entry(8, "help", 0);
    command_register_add(new_entry);
    // Command clear
    new_entry = create_command_entry(9, "clear", 0);
    command_register_add(new_entry);



}

int command_search(char* command_value)
{
    command_entry *iter = (command_entry *)malloc(sizeof(command_entry *));
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
                return command_cd(c_data, statement);

        case 4: // user
                return command_user(c_data, statement);

        case 5: // echo
                return command_echo(c_data, statement);

        case 6: // mkdir
                return command_mkdir(c_data, statement);

        case 7: // file
                return command_file(c_data, statement);

        case 8: // file
                return command_help(c_data, statement);

        case 9: // cd
                return command_clear(c_data, statement);

        default: // none
                 return -1;
    }
}

int command_registry_printer()
{
    printf("Available commands:\n");
    sleep(2);
    command_entry *iter = (command_entry *)malloc(sizeof(command_entry *));
    iter = command_registry_head;
    if(iter->_NEXT == NULL)
    {
        //printf("%s\n", iter->_command);
    }
    else
    {
        while(iter->_NEXT != NULL)
        {
            iter = iter->_NEXT;
            CONSOLE_PRINT("%s", iter->_command);
        }
    }
    free(iter);
}

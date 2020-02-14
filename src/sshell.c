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

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<termios.h>
#include<stdarg.h>
#include"util.h"
#include"command_handler.h"
#include"statement_handler.h"

int statementIndex = 0;
char statement[1024];

char getch()
{
    char buf = 0;
    buf = getchar();
    if('\033' == buf)
    {
        // printf("\nArrow Key?\n");
        getchar();
        buf = getchar();
        switch(buf)
        {
            case 'A':   LOG("%s", "Arrow Key Up!");
                        UP_ARROW_COUNT++;
                        process_up_arrow();
                        break;

            case 'B':   LOG("%s", "Arrow Key Down!");
                        DOWN_ARROW_COUNT--;
                        process_down_arrow();
                        break;

            case 'C':   LOG("%s", "Arrow Key Right!");
                        break;

            case 'D':   LOG("%s", "Arrow Key Left!");
                        break;
        }
        return '\0';
    }
    else
    {
        write(0, &buf, 1);
        return (buf);
    }
}

int process_down_arrow()
{
    char ch;
    // printf("\nUP_ARROW_COUNT: %d", UP_ARROW_COUNT);
    if(asl_counter == 0)
    {
        CONSOLE_PRINT("%s", "");
        return;
    }
    if(0 != DOWN_ARROW_COUNT)
    {
        asl_iterator_move_backward();
    }
    printf("%c[2K", 27);
    printf("\r");

    char *archiveStatement = (char *)malloc(sizeof(char *) * 1024);
    strcpy(archiveStatement, get_empty_array(2014));
    LOG("%s%s", "Arrow Up statement initialize: ", archiveStatement);
    strcpy(archiveStatement, asl_get_iter_string());
    LOG("%s%s", "Arrow Up statement copy: ", asl_iterator->_value);
    //printf("\narchive statement: %s", archiveStatement);
    print_prompt_and_statement(archiveStatement);

    ch = getch();
    if(ch == '\n' )
    {
        // If its a new line
        strcpy(statement, get_empty_array(1024));
        strcpy(statement, archiveStatement);
        statementIndex = strlen(statement);
        return process_statement();
    }

}

int process_up_arrow()
{
    char ch;
    // printf("\nUP_ARROW_COUNT: %d", UP_ARROW_COUNT);
    if(asl_counter == 0)
    {
        CONSOLE_PRINT("%s", "");
        return;
    }
    if(asl_counter <= UP_ARROW_COUNT)
    {

        asl_iterator_move_forward();
    }
    printf("%c[2K", 27);
    printf("\r");

    char *archiveStatement = (char *)malloc(sizeof(char *) * 1024);
    strcpy(archiveStatement, get_empty_array(2014));
    LOG("%s%s", "Arrow Up statement initialize: ", archiveStatement);
    strcpy(archiveStatement, asl_get_iter_string());
    LOG("%s%s", "Arrow Up statement copy: ", asl_iterator->_value);
    //printf("\narchive statement: %s", archiveStatement);
    print_prompt_and_statement(archiveStatement);

    ch = getch();
    if(ch == '\n' )
    {
        // If its a new line
        strcpy(statement, get_empty_array(1024));
        strcpy(statement, archiveStatement);
        statementIndex = strlen(statement);
        return process_statement();
    }

}

int process_statement()
{
    command_data* c_data;
    // If return key is encountered then revert UP_ARROW, and start processing statement
    UP_ARROW_COUNT = 0;
    DOWN_ARROW_COUNT = asl_counter;
    // resetting asl iterator
    asl_iterator_reset();
    statement[statementIndex] = '\0';
    LOG("%s: %s", "Statement", statement);
    if(check_if_return(statement))
    {
        LOG("%s", "Carriage Return Detected. Skipping processing!");
        // free(statement);
        return 1;
    }
    asl_add(statement);
    c_data = tokenize_statement(statement);
    statement_token_printer(c_data);
    return process_command(c_data, statement);
    // free(c_data);
}



int terminal_spawn()
{
    while(1)
    {
        char ch;
        statementIndex = 0;
        prompt();
        fflush(stdin);
        strcpy(statement, get_empty_array(1024));
        while(1)
        {
            // routine that handles character input and does preprocessing of arrow keys
            ch = getch();
            if('\0' == ch || '\n' == ch || '\r' == ch || 10 == ch)
            {
                break;
            }
            else
            {
                statement[statementIndex++] = ch;
            }

            fflush(stdin);

        }
        if(process_statement() == 0)
            break;
    }
    return 1;
}

int main()
{
    // setup logger
    logger_setup();
    LOG("%s", "\nStarting sshell session\n");
    CONSOLE_PRINT("%s", "\nStarting sshell session!\n");
    // Initialize Termios terminal
    terminal_init();
    LOG("%s", "Initialized Terminal");
    // asl is archived statement list. We are storing the statements in a list for history command
    asl_init();
    //LOG("Initialized ASL (Archived Statement List)");
    // This is used to initialize master command list. Here we can hook commands which will be used for processing
    command_register_init();

    // command_registry_printer();
    //LOG("Initialized Commands");
    // Spawn sshell
    //LOG("Spawning Terminal");
    terminal_spawn();
    terminal_reset();
    LOG("%s", "\nStopping sshell session\n");
    CONSOLE_PRINT("%s", "\nStopping sshell session!\n");
    return 0;
}

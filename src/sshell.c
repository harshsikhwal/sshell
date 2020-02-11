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


void process_up_arrow()
{
    // printf("\nUP_ARROW_COUNT: %d", UP_ARROW_COUNT);
    /*
    char *archiveStatement;
    if(1 == UP_ARROW_COUNT)
    {
        asl_iterator_reset();
    }
    asl_iterator_move();
    archiveStatement = asl_get_iter_string();
    //printf("\narchive statement: %s", archiveStatement);
    if(0 != strlen(archiveStatement))
    {
        //printf("%c[2K", 27);
        print_prompt_and_statement(archiveStatement);

    }
    else
    {
        // do nothing
    }
    */
}

char getch()
{
    char buf = 0;
    buf = getchar();
    if('\033' == buf)
    {
        printf("\nArrow Key?\n");
        getchar();
        buf = getchar();
        switch(buf)
        {
            case 'A':   printf("\nArrow Key UP!\n");
                        UP_ARROW_COUNT++;
                        process_up_arrow();
                        break;

            case 'B':   printf("\nArrow Key Down!\n");
                        break;

            case 'C':   printf("\nArrow Key Right!\n");
                        break;

            case 'D':   printf("\nArrow Key Left!\n");
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



int terminal_spawn()
{
    char statement[4096];
    char *command;
    command_data* c_data;
    while(1)
    {
        char ch;
        int statementIndex = 0;
        prompt();
        fflush(stdin);
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
        // If return key is encountered then revert UP_ARROW, and start processing statement
        UP_ARROW_COUNT = 0;
        statement[statementIndex] = '\0';
        asl_add(statement);
        c_data = tokenize_statement(statement);
        // command_data_printer(c_data);
        if(process_command(c_data, statement) == 0)
            break;
    }
}

int main()
{
    // Initialize Termios terminal
    terminal_init();
    // asl is archived statement list. We are storing the statements in a list for history command
    asl_init();
    // This is used to initialize master command list. Here we can hook commands which will be used for processing
    command_register_init();
    // Spawn sshell
    terminal_spawn();
    terminal_reset();
    return 0;
}

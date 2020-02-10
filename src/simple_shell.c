#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<termios.h>
#include<stdarg.h>
#include"util.h"
#include"command_registry.h"
#include"statement_tokenizer.h"


void processUPArrow()
{
    printf("\nUP_ARROW_COUNT: %d", UP_ARROW_COUNT);
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
        printPromptWithStatement(archiveStatement);

    }
    else
    {
        // do nothing
    }
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
            case 'A':   //printf("\nArrow Key UP!\n");
                        UP_ARROW_COUNT++;
                        processUPArrow();
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
    char statement[256];
    char *command;
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
        process_statement(statement);
        command = get_command(statement);
        if(process_command(command, statement) <= 0)
            break;
        free(command);
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

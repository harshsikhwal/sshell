#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<termios.h>
#include"util.h"


char cwd[256];

void processUPArrow()
{
    if(0 == UP_ARROW_COUNT)
    {
        resetACLIterator();
        moveACLIterator();
        getACLIteratorString();
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
            case 'A':   printf("\nArrow Key UP!\n");
                        UP_ARROW_COUNT++;
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

void processStatement(char *statement)
{
    int i;
    for(i=0; i<strlen(statement); i++)
    {
        if(statement[i] == '\n' || statement[i] == '\r')
        {
            statement[i] = '\0';
            break;
        }
    }
}

char* getCommand(char *statement)
{
    int i;
    char *command;
    int count = 0;

    //printf("Statement %s size : %d\n", statement, strlen(statement));

    for(i = 0; i < strlen(statement); i++)
    {
        if(statement[i] != ' ')
        {
            count++;
        }
        else
            break;
    }
/*
    if( i != count)
    {
        count++;
    }
*/
    command  = (char *)malloc(count + 1);
    strncpy(command, statement, count);
    //printf("Command: %s size: %d ", command, strlen(command));
    return (char *)command;
}

int echoStatement(char *statement)
{
    printf("%s\n", statement);
    fflush(stdin);
    return 1;
}

int printPrompt()
{
    char *pwd;
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        //printf("Working dir: %s", cwd);
        pwd = (char *)malloc(sizeof(cwd) + 2);
        strcpy(pwd, cwd);
        //printf("Working dir: %s", pwd);
        strcat(pwd, ">");
        //printf("Working dir: %s", pwd);
        printf("%s", pwd);
        //free(&pwd);
        return 1;
    }
    else
    {
        return -1;
    }
}

int printToConsole(char *str)
{
    printf("%s\n", str);
}

int processCommand(char *command, char *statement)
{
    if(strcmp(command, "echo") == 0)
    {
        echoStatement(statement);
    }
    else if(strcmp(command, "\n") == 0)
    {
        return 1;
    }
    else if(strcmp(command, "exit") == 0)
    {
        return 0;
    }
    else if(strcmp(command, "clear") == 0)
    {
        system("clear");
        return 1;
    }
    else if(strcmp(command, "user") == 0)
    {
        printToConsole(getlogin());
        return 1;
    }
    else if(strlen(command) == 0)
    {
        // do nothing
        return 2;
    }
    else if(strcmp(command, "history")==0)
    {
        //routine to print history
        printACL();
        return 1;
    }
    else
    {
        printToConsole(statement);
        printf("'%s' is not recognized as an internal or external command!\n", statement);
    }
}

int main()
{
    char statement[256];
    char *command;
    char ch;
    int statementIndex = 0;
    initTerminal();
    initializeACL();
    while(1)
    {
        printPrompt();
        fflush(stdin);
        while(1)
        {
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
        UP_ARROW_COUNT = 0;
        statement[statementIndex] = '\0';
        add2ACL(statement);
        statementIndex = 0;
        processStatement(statement);
        command = getCommand(statement);
        if(processCommand(command, statement) <= 0)
            break;
        free(command);
    }
    resetTerminal();
    return 0;
}

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<termios.h>
#include"util.h"
#include"command_registry.h"

char cwd[256];

int printPromptWithStatement(char *statement)
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
        printf("%s%s", pwd, statement);
        //free(&pwd);
        return 1;
    }
    else
    {
        return -1;
    }
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


void processUPArrow()
{
    printf("\nUP_ARROW_COUNT: %d", UP_ARROW_COUNT);
    char *archiveStatement;
    if(1 == UP_ARROW_COUNT)
    {
        acl_iterator_reset();
    }
    acl_iterator_move();
    archiveStatement = acl_get_iter_string();
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

int console_print(char *str)
{
    printf("%s\n", str);
}

int process_command(char *command, char *statement)
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
        console_print(getlogin());
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
        acl_print();
        return 1;
    }
    else
    {
        console_print(statement);
        printf("'%s' is not recognized as an internal or external command!\n", statement);
    }
}

int main()
{
    char statement[256];
    char *command;
    char ch;
    int statementIndex = 0;
    terminal_init();
    acl_init();
    command_register_init();
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
        acl_add(statement);
        statementIndex = 0;
        processStatement(statement);
        command = getCommand(statement);
        if(process_command(command, statement) <= 0)
            break;
        free(command);
    }
    terminal_reset();
    return 0;
}

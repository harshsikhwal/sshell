#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<curses.h>

char cwd[256];

void checkArrowKey(char *statement)
{
    if(statement[0] == '\033')
    {
        switch(statement[2])
        {
            case 'A' :
                printf("Arrow Key UP!");
                break;
        }
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
    printf("%s", statement);
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
        printw("%s", pwd);
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
    printw("%s\n", str);
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
    else
    {
        printToConsole(statement);
        printf("'%s' is not recognized as an internal or external command!\n", statement);
    }
}

int main()
{
    char statement[300];
    char *command;
    char ch;
    int index = 0;
    initscr();
    cbreak();
    while(1)
    {
        printPrompt();

        ch = getch();
        index = 0;
        while(ch != '\n')
        {
            if(ch == KEY_UP)
            {
                printw("UP Key is pressed!");
                break;
            }
            else
            {
                statement[index++] = ch;
            }
            ch = getch();
            //putc('*', stdin);
        }
        if(ch == '\r')
        {
            statement[index] = '\0';
        }
        //refresh();
    }
    return 0;
}

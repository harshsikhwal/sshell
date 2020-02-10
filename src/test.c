#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
//#include<curses.h>
#include<termios.h>


char cwd[256];

char getch()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
            perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror ("tcsetattr ~ICANON");
    return (buf);
}

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
    //cbreak();
    while(1)
    {
        printPrompt();
        /*
        if(getc(stdin) == '\033')
        {
            getc(stdin);
            getc(stdin);
            printf("Arrow Key Detected!");
            fflush(stdin);
            break;
        }
        */
        fflush(stdin);
        if(getch() == '\033')
        {
            getch();
            getch();
            printf("Arrow Key Detected!");
            //fflush(stdin);
        }
        else
        {
            fgets(statement, sizeof(statement), stdin);
            processStatement(statement);
            command = getCommand(statement);
            if(processCommand(command, statement) <= 0)
                break;
            free(command);
        }
        fflush(stdin);
    }
    return 0;
}

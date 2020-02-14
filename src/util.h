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

#define LOG(format, ...) \
        { \
            FILE *fp = fopen(logger_file, "a"); \
            fprintf(fp, "%s\t" format "\n", __TIME__, ##__VA_ARGS__); \
            fclose(fp); \
        }

#define CONSOLE_PRINT(format, ...) \
        { \
            fprintf(stdout, format "\n", ##__VA_ARGS__); \
            fflush(stdout); \
            fflush(stdin); \
        }

// asl counter

unsigned int asl_counter;

// Current working Directory
char cwd[256];

// Logger path

char *logger_file = "logger.log";

// ARROW COUNTERS

int UP_ARROW_COUNT = 0;
int DOWN_ARROW_COUNT = 0;
int LEFT_ARROW_COUNT = 0;
int RIGHT_ARROW_COUNT = 0;

struct termios original = {0};

struct command_data_struct
{
    char _command[256];
    char _flags[128][128];
    char _values[128][256];
    unsigned int _flags_count;
    unsigned int _values_count;
};

typedef struct command_data_struct command_data;

struct archive_statement_struct
{
    char _value[1024];
    struct archive_statement_struct *_LLINK;
    struct archive_statement_struct *_RLINK;
};

typedef struct archive_statement_struct archived_statements;

archived_statements *asl_head = NULL;
archived_statements *asl_iterator = NULL;

int logger_setup()
{
    if(access(logger_file, F_OK ) != -1 )
    {
        // file exists
        return remove(logger_file);
    }
    else
    {
        // do nothing
        return 1;
    }
}


char* get_empty_array(int size)
{
    char *temp_array = (char *)malloc(sizeof(char *) * size);
    int i = 0;
    for(; i < size - 1; i++)
    {
        temp_array[i] = '\0';
    }
    return temp_array;
}


// Terminal Functions
int terminal_init()
{
    CONSOLE_PRINT("%s", "\nInitializing Terminal....\n");
    LOG("%s", "Initializing Terminal....");
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    if (tcgetattr(0, &original) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
    {
        perror("tcsetattr ICANON");
        return -1;
    }
    CONSOLE_PRINT("%s", "\nDone Initializing Terminal\n");
    LOG("%s", "Done Initializing Terminal");
    return 1;
}

int terminal_reset()
{
    CONSOLE_PRINT("%s", "\nResetting Terminal....\n");
    LOG("%s", "Resetting Terminal....");
    if (tcsetattr(0, TCSANOW, &original) < 0)
    {
        perror("tcsetattr ICANON");
        return -1;
    }
    CONSOLE_PRINT("%s", "\nReset Terminal Complete\n");
    LOG("%s", "Reset Terminal Complete");
    return 1;
}


// Archived Command List Functions
void asl_init()
{
    asl_head = (archived_statements *)malloc(sizeof(archived_statements *));
    asl_head->_RLINK = NULL;
    asl_head->_LLINK = NULL;
    strcpy(asl_head->_value, "SIGNATURE BEGIN");
    asl_iterator = (archived_statements *)malloc(sizeof(archived_statements *));
    asl_iterator = asl_head;
    asl_counter = 0;
}


void asl_add(char *value)
{
    if((strcmp(value, "\n") == 0) || (strcmp(value, "history") == 0) || (strcmp(value, "\0") == 0))
        return;
    LOG("%s%s%s", "Adding \"", value, "\" to asl");
    archived_statements *newarchived_statements = (archived_statements *)malloc(sizeof(archived_statements *));
    archived_statements *iter = (archived_statements *)malloc(sizeof(archived_statements *));
    // Copying empty array to newarchived_statements
    strcpy(newarchived_statements->_value, get_empty_array(1024));
    strcpy(newarchived_statements->_value, value);
    // Iterate the asl
    if(asl_head->_RLINK == NULL)
    {
        asl_head->_RLINK = newarchived_statements;
        newarchived_statements->_LLINK = asl_head;
        newarchived_statements->_RLINK = NULL;
        asl_counter++;
    }
    else
    {
        iter = asl_head;
        while(NULL != iter->_RLINK)
        {
            iter = iter->_RLINK;
        }
        iter->_RLINK = newarchived_statements;
        newarchived_statements->_LLINK = iter;
        newarchived_statements->_RLINK = NULL;
        asl_counter++;
    }
    LOG("%s%s%s", "Statement: \"", value, "\" added");
}

void asl_print()
{
    archived_statements *iter;
    if(NULL == asl_head->_RLINK)
        return;
    else
    {
        iter = asl_head->_RLINK;
        while(NULL != iter->_RLINK)
        {
            CONSOLE_PRINT("%s", iter->_value);
            iter = iter->_RLINK;
        }
        CONSOLE_PRINT("%s", iter->_value);
    }
    free(iter);
}

char* asl_get_iter_string()
{
    if(asl_iterator == asl_head)
    {
        return "\0";
    }
    else
    {
        return asl_iterator->_value;
    }
}

void asl_iterator_move_forward()
{
    if(NULL == asl_iterator->_RLINK)
    {
        // DO NOTHING
    }
    else
    {
        asl_iterator = asl_iterator->_RLINK;
    }
}

void asl_iterator_move_backward()
{
    if(NULL == asl_iterator->_LLINK)
    {
        // DO NOTHING
    }
    else
    {
        asl_iterator = asl_iterator->_LLINK;
    }
}

void asl_iterator_reset()
{
    // asl_counter = 0;
    asl_iterator = NULL;
    asl_iterator = (archived_statements *)malloc(sizeof(archived_statements *));
    asl_iterator = asl_head;
}

void asl_delete()
{
    archived_statements *current, *next;
    // asl empty
    if(NULL == asl_head->_RLINK)
        return;
    else
    {
        current = asl_head->_RLINK;
        while(NULL != current)
        {
            next = current->_RLINK;
            free(current);
            current = next;
        }
    }
    free(current);
    asl_init();
    asl_iterator_reset();
    asl_counter = 0;
}

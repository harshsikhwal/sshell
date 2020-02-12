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
#include"prompt.h"


char* get_command(char *statement)
{
    int i;
    char *command;
    int count = 0;
    for(i = 0; i < strlen(statement); i++)
    {
        if(statement[i] != ' ')
        {
            count++;
        }
        else
            break;
    }
    command  = (char *)malloc(count + 1);
    strncpy(command, statement, count);
    //printf("Command: %s size: %d ", command, strlen(command));
    return (char *)command;
}

int process_command(command_data* c_data, char* statement)
{
    if(strlen(c_data->_command) == 0)
    {
        // do nothing
        return 2;
    }
    else if(strcmp(c_data->_command, "\n") == 0)
    {
        return 1;
    }
    unsigned command_id = command_search(c_data->_command);
    if(command_id > 0)
    {
        return master_command_handler(command_id, c_data, statement);
    }
    else
    {
        printf("'%s' is not recognized as an internal or external command!\n", c_data->_command);
        return 1;
    }
}

char* trim_statement(char *input_statement)
{
    int i = 0, new_counter = 0;
    char* modified_statement;
    int input_statement_size = strlen(input_statement);
    modified_statement = (char *) malloc(strlen(input_statement));
    if(' ' == input_statement[i])
    {
        // lookahead spaces
        while(1)
        {
            if(' ' != input_statement[i])
                break;
            i++;
        }
    }
    // Copy the statement from
    for( ; i < input_statement_size; i++)
    {
        modified_statement[new_counter++] = input_statement[i];
        // If space is encountered, store and look ahead
        if(' ' == input_statement[i])
        {
            // Go for a lookahead
            while(' ' == input_statement[i])
            {
                if(i == input_statement_size)
                {
                    break;
                }
                i++;
            }
            i--;
        }

        if( i == strlen(input_statement))
        {
            modified_statement[new_counter++] = '\0';
            break;
        }
    }
    //TODO: Check the end spaces
    return (char *)modified_statement;
}

command_data* tokenize_statement(char *statement)
{
    // Assuming the statement begins with a command
    command_data* c_data = (command_data *)malloc(sizeof(command_data *));
    int i;
    //char temp_str[256];
    int counter = 0;
    int flag_row_counter = 0;
    int value_row_counter = 0;
    char* trimmed_statement = (char * )malloc(sizeof(char *) * 256);
    trimmed_statement = trim_statement(statement);

    // extracting command

    for(i = 0; i < strlen(trimmed_statement); i++)
    {
        if(' ' == trimmed_statement[i])
        {
            break;
        }
        c_data->_command[counter++] = trimmed_statement[i];
    }
    c_data->_command[counter++] = '\0';
    counter = 0;

    for(i = i + 1; i<strlen(trimmed_statement); i++)
    {
        // Flag check
        if('-' == trimmed_statement[i] && ' ' == trimmed_statement[i-1] && ' ' != trimmed_statement[i+1])
        {
            i++;
            counter = 0;
            while(' ' != trimmed_statement[i])
            {
                c_data->_flags[flag_row_counter][counter++] = trimmed_statement[i];
                i++;
                if(i == strlen(trimmed_statement))
                {
                    break;
                }
            }
            c_data->_flags[flag_row_counter][counter++] = '\0';
            flag_row_counter++;
        }
        // Skip space
        else if(' ' == trimmed_statement[i])
        {
            continue;
        }
        else
        {
            // add value to value field in struct
            counter = 0;
            while(' ' != trimmed_statement[i])
            {
                c_data->_values[value_row_counter][counter++] = trimmed_statement[i];
                i++;
                if(i == strlen(trimmed_statement))
                {
                    break;
                }
            }
            c_data->_values[value_row_counter][counter++] = '\0';
            value_row_counter++;
        }
    }
    c_data->_flags_count = flag_row_counter;
    c_data->_values_count = value_row_counter;
    return c_data;
}

void command_data_printer(command_data* c_data)
{
    int i = 0;
    printf("\nPrinting Tokenized statement: ");
    printf("\nCommand: %s", c_data->_command);
    printf("\nNumber of flags: %d\nNumber of Values: %d", c_data->_flags_count, c_data->_values_count);
    printf("\nFlags:");
    for(i = 0; i < c_data->_flags_count + 1; i++)
    {
        printf("\n%s", c_data->_flags[i]);
    }
    printf("\nValues:");
    for(i = 0; i < c_data->_values_count + 1; i++)
    {
        printf("\n%s", c_data->_values[i]);
    }
}

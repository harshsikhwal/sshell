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

 #include <sys/stat.h>
 #include <sys/types.h>

int command_history(command_data* c_data, char* statement)
{
    //routine to print history
    // one flag at a time
    if (c_data->_flags_count == 1)
    {
        // Check flag
        // clear history
        if(strcmp(c_data->_flags[0], "c") == 0)
        {
            asl_delete();
        }
        else
        {
            CONSOLE_PRINT("%s", "Invalid flag entered");
        }
    }
    else
    {
        asl_print();
    }
    return 1;
}

int command_exit(command_data* c_data, char* statement)
{
    return 0;
}

int command_clear(command_data* c_data, char* statement)
{
    system("clear");
    return 1;
}

int command_user(command_data* c_data, char* statement)
{
    CONSOLE_PRINT("%s", getlogin());
    return 1;
}

int command_echo(command_data* c_data, char* statement)
{
    // Do echo preprocessing
    CONSOLE_PRINT("%s", statement);
    return 1;
}

int command_mkdir(command_data* c_data, char* statement)
{
    int i = 0;
    int return_value;
    for(i = 0; i < c_data->_values_count; i++)
    {
        // A temporary hack, need to fix this
        return_value = mkdir(c_data->_values[i], 777);
        // printf("\nReturn value: %d", return_value);
    }
    if(return_value == 0)
        return 1;
    else
        return return_value;
}


int command_file(command_data* c_data, char* statement)
{
    int i = 0;
    FILE *fptr;
    for(i = 0; i < c_data->_values_count; i++)
    {
        fptr = fopen(c_data->_values[i], "w");
        if(fptr == NULL)
        {
            CONSOLE_PRINT("%s", "Unable to create file.");
        }
        else
        {
            CONSOLE_PRINT("%s", "File created successfully");
            fclose(fptr);
        }
    }
    return 1;
}

int command_help(command_data* c_data, char* statement)
{
    command_registry_printer();
    return 1;
}

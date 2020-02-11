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


int command_history(command_data* c_data, char* statement)
{
    //routine to print history
    // one flag at a time
    int flag_limit = 1;
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
            console_print("Invalid flag entered");
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
    console_print(getlogin());
    return 1;
}

int command_echo(command_data* c_data, char* statement)
{
    // Do echo preprocessing
    console_print(statement);
    return 1;
}

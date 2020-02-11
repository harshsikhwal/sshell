

/*
1. history
2. exit
3. clear
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

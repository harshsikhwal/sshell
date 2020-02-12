# sshell

A simple shell written in C where you can hook your commands easily and play with it.

Folder structure:
* sshell
	* src
		* commands
	 

This code is divided into following files:
* src/simple_shell.c
* src/command_handler.h
* src/prompt.h
* src/statement_handler.h
* src/util.h
* src/commands/standard_commands.h

Libraries used:
stdio.h
string.h
stdlib.h
unistd.h
limits.h
termios.h
stdarg.h

sshell contains the main executable. The code initializes terminal (termios), initializes asl (archived statement list) required for history and initializes commands and spawns the shell (prompt with the current path)

The user can enter "statement" and execute commands as described in command_handler.h

A "statement" is comprised of "command", "flags" and "values". The first word being the command.

Once a statement is entered by the user, it is tokenized to command, flags and values in statement_handler.h. Each of this command is stored in asl or archived statement list which is required by "history" command.

After preprocessing and tokenization, the command is checked against the entries as specified by the programmer.

How can we hook commands?

This code uses a common data structure which can be used by any subroutine specific to a command. The data structure:

```
struct command_data_struct
{
    char _command[256];
    char _flags[128][128];
    char _values[128][256];
    unsigned int _flags_count;
    unsigned int _values_count;
};
```

This contains the command, flags and values in sequence in which they were written by the user. This can be passed to other command handling routines.

The commands can be hooked into the code in command_handler.h:

```
void command_register_init()
```

In the abve function, you can add the command in list:

```
// Command exit
new_entry = create_command_entry(1, "exit", 0);
command_register_add(new_entry);
```
You need to assign a unique "id" as specified in the first argument. The second argument is the command itself. The third is the number of flags and the corresponding flags can be sent as parameters (using stdarg.h for variable argument)

Example of variable argument:

```
// Command history
new_entry = create_command_entry(2, "history", 1, "c");
command_register_add(new_entry);
```

The unique id must be unique to each command. Using this id, we will call the specific subroutine and pass the data.

```
int master_command_handler(int command_id, command_data* c_data, char* statement)
```

The above function has a switch block that has various cases and it switches based on the command_id. The unique id will be used here to call specific subroutine:

```
case 1: // exit
        return command_exit(c_data, statement);
```

Here the routine ```command_exit(c_data, statement); ``` is present in commands/standard_commands.h file. Therefore the programmer can define his command.h file, give a jump to the routine using the unique id.

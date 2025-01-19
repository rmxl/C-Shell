# My Shell

This is my C shell built from scratch.

# Instructions

### To compile:

`make`

### To run:

`./a.out`

### To quit:

`exit`

## Details
- ```;``` , ```||``` , ```&```, ```<```, ```>``` and  ```>>```work just like in a normal shell.
- The shell considers the directory it is opened in as the home directory.
- Errors are displayed properly.
- Assumes that paths do not contain whitespace.
- Time taken by the foreground process and the name of the process are printed in the next prompt if process takes > 2 seconds to run.
- ```.myshrc``` : a shell file where you can define your custom functions and aliases. For example,
  
```
// Function
  mk_hop() 
{ 
	mkdir "$1" # Create the directory 
	hop "$1" # Change into the directory 
}

// Alias
alias home = hop ~
```

## Commands
1. **hop**
- Works just like the cd command, with all the flags such as ``.``, ``..``, etc.
- Can chain the directories.
2. **reveal**
- Works just like the ```ls``` command, with support for -l and -a flags.
3. **log**
- Similar to the history commands in bash.
- ```log purge``` : Clears all the logs currently stored.
- ```log execute <index>``` : Executes the command at the specified index.
4. **proclore <pid>**
- Prints information regarding a process, including the pid, process status, process group, virtual memory and path of process.
- If no argument specified, shows the information of the shell.
5. **seek <target> <location>**
- ```seek``` command looks for a file/directory in the specified target directory (or current if no directory is specified). It returns a list of relative paths (from target directory) of all matching files/directories separated with a newline character.
- ```-d``` : Only looks for directories, ```-f``` : Only looks for files, ```-e```: This flag is effective only when a single file or a single directory with the name is found. If only one file (and no directories) is found, then prints it’s output. If only one directory (and no files) is found, then changes current working directory to it. Otherwise, the flag has no effect.
6. **ping <pid> <signal_number>**
- ping command is used to send signals to processes. Take the pid of a process and send a signal to it which corresponds to the signal number (which is provided as an argument).
- Following 3 commands are direct keyboard input where Ctrl is Control key on keyboard (or it’s equivalent).
- Ctrl - C : Interrupt any currently running foreground process by sending it the SIGINT signal. It has no effect if no foreground process is currently running.

Ctrl - D : Log out of your shell (after killing all processes) while having no effect on the actual terminal.

Ctrl - Z : Push the (if any) running foreground process to the background and change it’s state from “Running” to “Stopped”. It has no effect on the shell if no foreground process is running.

7. **fg <pid>**
- Brings the running or stopped background process with corresponding pid to foreground, handing it the control of terminal.

8. **bg <pid>**
- Changes the state of a stopped background process to running (in the background)

9. **neonate -n [time_arg]**
- The command prints the Process-ID of the most recently created process on the system (you are not allowed to use system programs), this pid will be printed every [time_arg] seconds until the key ‘x’ is pressed.
10. **iMan <command>**
- iMan fetches man pages from the internet and outputs it to the terminal.

## Assumptions

- Maximum input length, and other lengths is 4096 characters.
- Maximum path length is 1024 characters.
- Implemented an extra exit function for exiting the shell.
- Seek commannd will also print time taken if it takes greater than 2 seconds.
- Any system command failure will cause the shell to stop executing the last set of commands.
- Invalid use of IO redirection should give a syntax error.
- `&` followed by pipe will given an error, as will pipe without a command preceding or succeeding it.
- If you want to access sleep(2) instead of sleep you can do that.
- For I/O redirection, only the following cases are possible:
  - no `<` or `>` or `>>`
  - one `<`
  - one `>`
  - one `>>`
  - one `<` and one `>`
  - one `<` and one `>>`
- In `hop`, 
    - `'-'` will take you to the last directory before current command string.
    - If hop is not provided with any arguments, it is assumed there will be no extra spaces.
    - If part of hop is invalid, it will hop to all the valid directories and then display an error message.
- In `log`,
    - `log` execute will store the command again provided it is not same as last command.
    - `log` won't store a string if it contains the `log` command.
    - `log` stores `.myShellHistory` in the /home/user directory.
- In .`myshrc`:
    - The syntax must be strictly followed, aliases must begin with `alias` and function names must end with `()` and have braces in their own lines.
    - In functions, command line arguments are done with `'$1'`, `'$2'` etc.
    - There is no strict error checking implemented for `.myshrc`.
- In `neonate`,
  - if non-integer values are entered as the time argument, they will be rounded to their floor value.
  - non-negative values will not be entered.
  - `neonate` assumes a default time of 5 seconds if argument is not given.
- In `activities`
    - The command name is stored without arguments.
    - The list is sorted according to the command names.

## Description

- get_input.c: for displaying the prompt and accepting input
- shell_prompt.c : for displaying the prompt
- execute.c : for parsing the commands and executing them
- ioredirect.c: for handling input/output redirection
- pipe.c: for handling pipes
- parser.c : for parsing the input
- exec.c : for executing system commands
- log.c : for logging the commands
- hop.c : for traversing directories
- color_print.c : for printing colored text
- customCommands.c : for handling custom commands as defined in .myshrc
- proclore.c: for displaying information regarding any process
- reveal.c : for displaying the contents of a directory
- fgbg.c : for handling the `fg` and `bg` commands
- seek.c: for displaying matching files and directories
- activities.c: for displaying all processes spawned by the shell
- ping.c : for sending signals to processes
- neonate.c: to print the most recent process on the system
- iman.c: for displaying man pages
- linkedlist.c : for handling linked lists used for storing bg processes

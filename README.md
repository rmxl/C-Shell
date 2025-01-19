# My Shell

This is my C shell built from scratch.

# Instructions

### To compile:

`make`

### To run:

`./a.out`

### To quit:

`exit`

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

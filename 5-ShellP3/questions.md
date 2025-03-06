1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

The highest level parent process of all of the shells loops through all of the child process ids and calls waitpid() on them. This waits until these processes are exited before contuning to run the program. If wait pid wasn't called on all child processes, the output to the console could be confusing because the output may not be in order of the commands called. 

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

When a pipe is closed, it signals and EOF and the process reading it will know to stop reading from the pipe. however, if EOF is never signaled then the process will read it indefintely. Also, if many unused pipes are created and not closed, it can use up system resources.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

When external processes are executed, we use fork and exec to run the process. This means we are running the command in a child process. If we change directories in a child process, this won't change the directory of parent process(the shell). So in order to change the dirctory of the shell, it needs to be called in the process that the shell is running in. 

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

My implementation currently uses malloc to allocate memory for cmd_buff. If the amount of commands is known, a fixed array without malloc can be used for the command list. This is faster as system calls don't need to be made. However, since there are only 8 commands, and each commands takes up a marginal amount of memory, it can be argued that dynamic memory management is not significantly slower than using the stack. 

To make things more efficent for an arbitrary amount of commands, the following changes can be made: When a process finishes running, the shell process can free the memory in the command list and command buff that was dedicated to that process. The shell may also wait for a certain number of proccesses to finishing running before starting more so that new processes don't inherit a large command list, and so that not too many processes are running at the same time. Essentially, the command list can be built while commands are being proccessed, and it will free old memory that is no longer being used as commands are run. To prevent too much memeory and resources being used at the same time, the building of the command list can be paused as it waits for previous processes to finish and for their memeory be freed. 

